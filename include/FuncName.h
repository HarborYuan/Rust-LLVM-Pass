//
// Created by Haobo on 2020/7/17.
//

#ifndef RUST_DCC_FUNCNAME_H
#define RUST_DCC_FUNCNAME_H

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

//------------------------------------------------------------------------------
// Legacy PM interface
//------------------------------------------------------------------------------
struct FuncName : public llvm::ModulePass {
    static char ID;
    FuncName() : ModulePass(ID) {}
    bool runOnModule(llvm::Module &M) override;
};



#endif //RUST_DCC_FUNCNAME_H
