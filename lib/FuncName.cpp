//
// Created by Haobo on 2020/7/17.
//

#include "FuncName.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/IRBuilder.h"

#include "demangle.h"

using namespace llvm;

//-----------------------------------------------------------------------------
// FuncName will run on the whole module.
// We need to scan all of the functions on it.
//-----------------------------------------------------------------------------

bool FuncName::runOnModule(llvm::Module &M) {
    // TODO
}


//-----------------------------------------------------------------------------
// Legacy PM Registration
//-----------------------------------------------------------------------------
char FuncName::ID = 0;

// Register the pass - required for opt
static RegisterPass<FuncName>
        X(/*PassArg=*/"func_name",
        /*Name=*/"Function Name Pass",
        /*CFGOnly=*/false,
        /*is_analysis=*/false);

static RegisterStandardPasses Y(
        PassManagerBuilder::EP_EarlyAsPossible,
        [](const PassManagerBuilder &Builder,
           legacy::PassManagerBase &PM) { PM.add(new FuncName()); });

