//
// Created by Haobo on 2020/7/17.
//


#include "llvm/Transforms/Utils/ModuleUtils.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/IRBuilder.h"


#include "Instrument/PaInstrument/PaInstrument.h"

using namespace llvm;
using namespace std;

static RegisterPass<PAInstrumentor> X(
	"instrument-params",
	"record parameter values for generic functions",
	false,
	true);


char PAInstrumentor::ID = 0;
const unsigned PAInstrumentor::InvalidID;

void PAInstrumentor::getAnalysisUsage(AnalysisUsage &AU) const {
	AU.setPreservesAll();
}

PAInstrumentor::PAInstrumentor(): ModulePass(ID) {}

void PAInstrumentor::print(raw_ostream &O, const Module *M ) const {}






bool PAInstrumentor::runOnModule(llvm::Module &M) {
    
}


