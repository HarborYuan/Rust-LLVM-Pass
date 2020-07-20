//
// Created by Haobo on 2020/7/17.
//

#include <string>
#include <map>
#include <set>

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


string getDemangledFuncName(string strFuncName) {
	return "";
}


map<string, set<Function*> > collectGenericFuns(Module & M) {

	map<string, set<Function *> > mapDemangledNames;

	for(auto FF = M.begin(); FF != M.end() ; FF ++ ) {
		string strDFuncName = getDemangledFuncName(FF->getName());

		if(mapDemangledNames.find(strDFuncName) == mapDemangledNames.end()) {
			set<Function * > setTmp;
			mapDemangledNames[strDFuncName] = setTmp;
		} 

		mapDemangledNames[strDFuncName].insert(&*FF);
	}	


	map<string, set<Function *> > mapResult;


	for(auto itMapBegin = mapDemangledNames.begin(); itMapBegin != mapDemangledNames.end(); itMapBegin ++) {
		if(itMapBegin->second.size() > 1 ) {
			mapResult[itMapBegin->first] = itMapBegin->second;
		}
	}

	return mapResult;
}


void PAInstrumentor::InitTypes(Module & M) {
	//insert needed type declaration
}

void PAInstrumentor::InitHooks(Module & M) {
	//insert hook function declaration into module m
}



void PAInstrumentor::print(raw_ostream &O, const Module *M ) const {

}


bool PAInstrumentor::runOnModule(llvm::Module &M) {
	InitTypes(M);
    InitHooks(M);

    map<string, set<Function *> > mapGenericFunctions = collectGenericFuns(M);

    for(auto itMapBegin = mapGenericFunctions.begin(); itMapBegin != mapGenericFunctions.end(); itMapBegin ++ ) {
    	for(auto pFF = itMapBegin->second.begin(); pFF != itMapBegin->second.end(); pFF ++ ) {
    		// if the function has integer parameters, then print the integer parameters
    	} 
    }

}


