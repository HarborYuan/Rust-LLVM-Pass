#ifndef _H_HAOBO_PAINSTRUMENT
#define _H_HAOBO_PAINSTRUMENT


#include "llvm/Pass.h"


using namespace llvm;

struct PAInstrumentor: public ModulePass {
	static char ID;
	static const unsigned InvalidID = -1;

	PAInstrumentor();
	virtual void getAnalysisUsage(AnalysisUsage & AU) const;
	virtual bool runOnModule(Module & M);
	virtual void print(raw_ostream &O, const Module *M) const;

private:
	void InitTypes(Module & M);
	void InitHooks(Module & M);


	// Types

    Type *VoidType;
    IntegerType *LongType;
    IntegerType *CharType;
    IntegerType *IntType;
    IntegerType *BoolType;

    PointerType *LongPointerType;
    PointerType *VoidPointerType;
    PointerType *CharPointerType;


    // Hook Funcs
    Function *PrintFuncName;
    Function *PrintIntParam;
    Function *PrintVoidParam;
    Function *PrintCharParam;

};



#endif