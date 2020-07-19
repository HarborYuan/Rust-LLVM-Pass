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
    bool Instrumented = false;
    auto &CTX = M.getContext();
    PointerType *PrintfArgTy = PointerType::getUnqual(Type::getInt8Ty(CTX));
    // 1. Build a printf function
    FunctionType *PrintfTy = FunctionType::get(
            IntegerType::getInt32Ty(CTX),
            PrintfArgTy,
            /*isVarArg=*/true);
    FunctionCallee Printf = M.getOrInsertFunction("printf", PrintfTy);
    auto *PrintfF = dyn_cast<Function>(Printf.getCallee());
    PrintfF->setDoesNotThrow();
    PrintfF->addParamAttr(0, Attribute::NoCapture);
    PrintfF->addParamAttr(0, Attribute::ReadOnly);
    // 2. Build a message string
    llvm::Constant *PrintfFormatStr = llvm::ConstantDataArray::getString(
            CTX, "(Function Name): %s -> number of arguments: %d\n");
    Constant *PrintfFormatStrVar =
            M.getOrInsertGlobal("PrintfFormatStr", PrintfFormatStr->getType());
    dyn_cast<GlobalVariable>(PrintfFormatStrVar)->setInitializer(PrintfFormatStr);
    // 3. Insert to each function
    for (auto &F : M) {
        if (F.isDeclaration())
            continue;
        IRBuilder<> Builder(&*F.getEntryBlock().getFirstInsertionPt());
        std::string demangle_name_std = rust_demangle(F.getName().str());
        Constant *FuncName = Builder.CreateGlobalStringPtr(demangle_name_std.c_str());
        llvm::Value *FormatStrPtr =
                Builder.CreatePointerCast(PrintfFormatStrVar, PrintfArgTy, "formatStr");

        Builder.CreateCall(
                Printf, {FormatStrPtr, FuncName, Builder.getInt32(F.arg_size())});
        Instrumented = true;
    }
    return Instrumented;
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

