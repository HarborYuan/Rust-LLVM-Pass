//
// Created by Haobo on 2020/7/19.
//

#include "demangle.h"
#include "rustc_demangle.h"
#include <algorithm>
#include <tuple>
#include <vector>
#include <iostream>

std::string rust_demangle(std::string input) {
    if (input == "main") {
        return input;
    }
    char output[255];
    int code = rustc_demangle(input.c_str(), output, input.size());
    if (code) {
        return output;
    } else {
        char output2[2550];
        int code2 = rustc_demangle(input.c_str(), output2, input.size());
        if (code2) {
            return output2;
        } else {
            return std::string("rust_demangle() :: Not a Rust Symbol => " + input);
        }
    }
}

std::tuple<std::string, bool> rust_demangle_legacy(std::string input) {
    if (input.compare(0, 3, "_ZN") == 0) {
        input = input.substr(3);
    } else if (input.compare(0, 4, "__ZN") == 0) {
        input = input.substr(4);
    } else if (input.compare(0, 2, "ZN") == 0) {
        input = input.substr(2);
    } else {
        // Not a legacy mangled string
        return {std::string(""), false};
    }


    // Check if there is non-ascii char
    auto pos = std::find_if(input.cbegin(), input.cend(),
                            [](unsigned char current) { return current & static_cast<unsigned char>(0x80); });
    if (pos != input.cend()) {
        // Not a legacy mangled string
        return {std::string(""), false};
    }

    std::vector<std::string> slices;
    auto iter = input.cbegin();
    while (iter != input.cend() && *iter != 'E') {
        if (!isdigit(*iter)) {
            // No num prefix
            return {std::string(""), false};
        }
        unsigned int len = 0;
        while (isdigit(*iter)) {
            len = len * 10 + (*iter - '0');
            iter++;
            if (iter == input.cend()) {
                return {std::string(""), false};
            }
        }
        std::string tmp;
        for (int i = 0; i < len; i++) {
            if (iter != input.cend()) {
                tmp.push_back(*iter++);
            } else {
                return {std::string(""), false};
            }
        }
        slices.push_back(tmp);
    }
    std::string result;
    for (auto iter = slices.cbegin(); iter != slices.cend(); iter++) {
        if (iter + 1 == slices.cend()){
            // Hash
            break;
        }
        if (iter != slices.cbegin()) {
            result += "::";
        }
        auto tmp = *iter;
        if (tmp.compare(0, 2, "_$") == 0) {
            tmp = tmp.substr(1); // Not a typo, just skip _
        }
        while (true) {
            if (tmp.length() >= 1 && tmp[0] == '.') {
                if (tmp.length() >= 2 && tmp[1] == '.') {
                    result += "::";
                    tmp = tmp.substr(2);
                } else {
                    result += ".";
                    tmp = tmp.substr(1);
                }
            } else if (tmp.length() >= 1 && tmp[0] == '$') {
                auto this_end = std::find(tmp.cbegin() + 1, tmp.cend(), '$');
                if (this_end != tmp.cend()) {
                    auto this_start = tmp.cbegin() + 1;
                    std::string sub_now;
                    for (auto iter_inner = this_start; iter_inner != this_end; iter_inner++) {
                        sub_now.push_back(*iter_inner);
                    }
                    std::string sub_now_unescaped;
                    if (sub_now == "SP") {
                        sub_now_unescaped = "@";
                    } else if (sub_now == "BP") {
                        sub_now_unescaped = "*";
                    } else if (sub_now == "RF") {
                        sub_now_unescaped = "&";
                    } else if (sub_now == "LT") {
                        sub_now_unescaped = "<";
                    } else if (sub_now == "GT") {
                        sub_now_unescaped = ">";
                    } else if (sub_now == "LP") {
                        sub_now_unescaped = "(";
                    } else if (sub_now == "RP") {
                        sub_now_unescaped = ")";
                    } else if (sub_now == "C") {
                        sub_now_unescaped = ",";
                    } else {
                        if (sub_now[0] == 'u') {
                            sub_now = sub_now.substr(1);
                            if (sub_now.find_first_not_of("0123456789abcdef") == std::string::npos) {
                                int x = std::stoi(sub_now, nullptr,16);
                                result.push_back(static_cast<char>(x));
                                tmp = tmp.substr(this_end + 1 - tmp.cbegin());
                                continue;
                            }
                        }
                        break;
                    }
                    result += sub_now_unescaped;
                    tmp = tmp.substr(this_end + 1 - tmp.cbegin());
                } else {
                    break;
                }
            } else {
                auto this_end = std::find_if(tmp.cbegin(), tmp.cend(), [](char c) {
                    return c == '.' || c == '$';
                });
                if (this_end != tmp.cend()) {
                    result += tmp.substr(0, this_end - tmp.cbegin());
                    tmp = tmp.substr(this_end - tmp.cbegin());
                } else {
                    break;
                }
            }
        }
        result += tmp;
    }
    return {result, true};
}

std::string rust_demangle_tmp(std::string input) {
    // strip out llvm linker mangling
    const auto llvm_symbol = std::string(".llvm.");
    std::size_t pos = input.find(llvm_symbol);
    if (pos != std::string::npos) {
        std::string candidate = input.substr(pos + llvm_symbol.length());
        if (candidate.find_first_not_of("0123456789ABCDEF@") != std::string::npos) {
            input = input.substr(0, pos);
        }
    }

    std::string res;
    bool is_legacy;
    std::tie(res, is_legacy) = rust_demangle_legacy(input);
    if (is_legacy) {
        return res;
    } else {
        return input;
    }
}