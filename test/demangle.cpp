//
// Created by 10278 on 2020/7/19.
//

#include "demangle.h"
#include <iostream>

int main() {
    std::cout << rust_demangle("_ZN3std2rt10lang_start17he7fd5b5ef5480c6fE") << std::endl;
    std::cout << rust_demangle("_ZN3std2rt10lang_start28_$u7b$$u7b$closure$u7d$$u7d$17h084a3aaa315ac765E") << std::endl;
    std::cout << rust_demangle("_ZN4main4test17hd63665aabd621732E") << std::endl;
    std::cout << rust_demangle(
            "_ZN68_$LT$std..process..ExitCode$u20$as$u20$std..process..Termination$GT$6report17h43ae8d12a006b358E")
              << std::endl;
    return 0;
}