//
// Created by Haobo on 2020/7/19.
//

#ifndef RUST_LLVM_PASS_DEMANGLE_H
#define RUST_LLVM_PASS_DEMANGLE_H

#include <string>

std::string rust_demangle(std::string input);

std::string rust_demangle_tmp(std::string input);


#endif //RUST_LLVM_PASS_DEMANGLE_H