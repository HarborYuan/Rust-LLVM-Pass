//
// Created by Haobo on 2020/7/19.
//

#include "demangle.h"
#include "rustc_demangle.h"

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
