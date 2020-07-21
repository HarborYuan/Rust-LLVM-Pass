//
// Created by 10278 on 2020/7/19.
//

#include "demangle.h"
#include <iostream>
#include <vector>

int main() {
    std::vector<std::string> input;
    input.emplace_back("_ZN3std2rt10lang_start17he7fd5b5ef5480c6fE");
    input.emplace_back("_ZN3std2rt10lang_start28_$u7b$$u7b$closure$u7d$$u7d$17h084a3aaa315ac765E");
    input.emplace_back("_ZN4main4test17hd63665aabd621732E");
    input.emplace_back("_ZN68_$LT$std..process..ExitCode$u20$as$u20$std..process..Termination$GT$6report17h43ae8d12a006b358E");

    for(auto & item : input){
        std::cout << rust_demangle_tmp(item) << std::endl;
        std::cout << rust_demangle(item) << std::endl;
    }
    return 0;
}