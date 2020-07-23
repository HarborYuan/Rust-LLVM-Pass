//
// Created by 10278 on 2020/7/19.
//

#include "demangle.h"
#include <iostream>
#include <vector>

int main() {
    std::vector<std::string> input{
            "_ZN3std2rt10lang_start17he7fd5b5ef5480c6fE",
            "_ZN3std2rt10lang_start28_$u7b$$u7b$closure$u7d$$u7d$17h084a3aaa315ac765E",
            "_ZN4main4test17hd63665aabd621732E",
            "_ZN68_$LT$std..process..ExitCode$u20$as$u20$std..process..Termination$GT$6report17h43ae8d12a006b358E",
            "_ZN4test1a2bcE",
            "_ZN4$RP$E",
            "_ZN8$RF$testE",
            "_ZN8$BP$test4foobE",
            "_ZN9$u20$test4foobE",
            "_ZN35Bar$LT$$u5b$u32$u3b$$u20$4$u5d$$GT$E",
            "_ZN13test$u20$test4foobE",
            "_ZN12test$BP$test4foobE",
            "__ZN5alloc9allocator6Layout9for_value17h02a996811f781011E",
            "__ZN38_$LT$core..option..Option$LT$T$GT$$GT$6unwrap18_MSG_FILE_LINE_COL17haf7cb8d5824ee659E",
            "__ZN4core5slice89_$LT$impl$u20$core..iter..traits..IntoIterator$u20$for$u20$$RF$$u27$a$u20$$u5b$T$u5d$$GT$9into_iter17h450e234d27262170E",
            "ZN12test$RF$test4foobE",
            "_ZN28_$u7b$$u7b$closure$u7d$$u7d$E",
            "_ZN13_$LT$test$GT$E",
            "_ZN15__STATIC_FMTSTRE",
            "_ZN71_$LT$Test$u20$$u2b$$u20$$u27$static$u20$as$u20$foo..Bar$LT$Test$GT$$GT$3barE",
            "_ZN3foo17h05af221e174051e9E",
            "_ZN3fooE",
            "_ZN3foo3barE",
            "_ZN3foo20h05af221e174051e9abcE",
            "_ZN17h05af221e174051e93fooE",
            "_ZN3foo17hg5af221e174051e9E",
            "_ZN3fooE.llvm.9D1C9369",
            "_ZN3fooE.llvm.9D1C9369@@16",
            "_ZN9backtrace3foo17hbb467fcdaea5d79bE.llvm.A5310EB9",
            "_ZN4core5slice77_$LT$impl$u20$core..ops..index..IndexMut$LT$I$GT$$u20$for$u20$$u5b$T$u5d$$GT$9index_mut17haf9727c2edfbc47bE.exit.i.i",
            "_ZN4core5slice77_$LT$impl$u20$core..ops..index..IndexMut$LT$I$GT$$u20$for$u20$$u5b$T$u5d$$GT$9index_mut17haf9727c2edfbc47bE.exit.i.i",
            "_ZN5*70527a54.ll34_$b.1E",
            "_ZNfooE",
            "_ZN151_$LT$alloc..boxed..Box$LT$alloc..boxed..FnBox$LT$A$C$$u20$Output$u3d$R$GT$$u20$$u2b$$u20$$u27$a$GT$$u20$as$u20$core..ops..function..FnOnce$LT$A$GT$$GT$9call_once17h69e8f44b3723e1caE",
            "_ZN88_$LT$core..result..Result$LT$$u21$$C$$u20$E$GT$$u20$as$u20$std..process..Termination$GT$6report17hfc41d0da4a40b3e8E",
            "_ZN11issue_609253foo37Foo$LT$issue_60925..llv$u6d$..Foo$GT$3foo17h059a991a004536adE",
//            "_ZN11utf8_idents157_$u10e1$$u10d0$$u10ed$$u10db$$u10d4$$u10da$$u10d0$$u10d3$_$u10d2$$u10d4$$u10db$$u10e0$$u10d8$$u10d4$$u10da$$u10d8$_$u10e1$$u10d0$$u10d3$$u10d8$$u10da$$u10d8$17h21634fd5714000aaE",
            "_ZN11issue_609253foo37Foo$LT$issue_60925..llv$u6d$..Foo$GT$3foo17h059a991a004536adE",
    };

    bool is_success = true;
    for (auto &item : input) {
        auto LHE = rust_demangle_tmp(item);
        auto RHE = rust_demangle(item);
        if (LHE == RHE) {
            continue;
        }
        is_success = false;
        std::cout << LHE << std::endl;
        std::cout << RHE << std::endl;
    }

    if (is_success) {
        std::cout << "OK!" << std::endl;
    }
    return 0;
}