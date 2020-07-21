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

#include "demangle.h"

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

PAInstrumentor::PAInstrumentor() : ModulePass(ID) {}


string getDemangledFuncName(string strFuncName) {
    return rust_demangle(strFuncName);
}


map<string, set<Function *> > collectGenericFuns(Module &M) {

    map<string, set<Function *> > mapDemangledNames;

    for (auto FF = M.begin(); FF != M.end(); FF++) {
        if (FF->isDeclaration())
            continue;
        string strDFuncName = getDemangledFuncName(FF->getName().str());

        if (mapDemangledNames.find(strDFuncName) == mapDemangledNames.end()) {
            set<Function *> setTmp;
            mapDemangledNames[strDFuncName] = setTmp;
        }

        mapDemangledNames[strDFuncName].insert(&*FF);
    }


    map<string, set<Function *> > mapResult;


    for (auto itMapBegin = mapDemangledNames.begin(); itMapBegin != mapDemangledNames.end(); itMapBegin++) {
        if (itMapBegin->second.size() > 1) {
            mapResult[itMapBegin->first] = itMapBegin->second;
        }
    }

    return mapResult;
}


void PAInstrumentor::InitTypes(Module &M) {
    this->VoidType = Type::getVoidTy(M.getContext());
    this->LongType = IntegerType::get(M.getContext(), 64);
    this->IntType = IntegerType::get(M.getContext(), 32);
    this->CharType = IntegerType::get(M.getContext(), 8);
    this->BoolType = IntegerType::get(M.getContext(), 1);

    this->VoidPointerType = PointerType::get(this->CharType, 0);
    this->CharPointerType = PointerType::get(this->CharType, 0);
    this->LongPointerType = PointerType::get(this->LongType, 0);
}

void PAInstrumentor::InitHooks(Module &M) {
    std::vector<Type *> ArgTypes;

    // PrintFuncName
    // void PrintFuncName(char *i);
    this->PrintFuncName = M.getFunction("PrintFuncName");
    if (!this->PrintFuncName) {
        ArgTypes.clear();
        ArgTypes.push_back(this->CharPointerType);
        FunctionType *PrintFuncName_FuncTy = FunctionType::get(this->VoidType, ArgTypes, false);
        this->PrintFuncName = Function::Create(PrintFuncName_FuncTy, GlobalValue::ExternalLinkage, "PrintFuncName", M);
        this->PrintFuncName->setCallingConv(CallingConv::C);
    } else {
        errs() << " Error when creating : " << "PrintFuncName" << "\n";
    }

    // PrintIntParam
    // void PrintIntParam(int i);
    this->PrintIntParam = M.getFunction("PrintIntParam");
    if (!this->PrintIntParam) {
        ArgTypes.clear();
        ArgTypes.push_back(this->IntType);
        FunctionType *PrintIntParam_FuncTy = FunctionType::get(this->VoidType, ArgTypes, false);
        this->PrintIntParam = Function::Create(PrintIntParam_FuncTy, GlobalValue::ExternalLinkage, "PrintIntParam", M);
        this->PrintIntParam->setCallingConv(CallingConv::C);
    } else {
        errs() << " Error when creating : " << "PrintIntParam" << "\n";
    }

    // PrintParamNum
    // void PrintParamNum(int i);
    this->PrintParamNum = M.getFunction("PrintParamNum");
    if (!this->PrintParamNum) {
        ArgTypes.clear();
        ArgTypes.push_back(this->IntType);
        FunctionType *PrintParamNum_FuncTy = FunctionType::get(this->VoidType, ArgTypes, false);
        this->PrintParamNum = Function::Create(PrintParamNum_FuncTy, GlobalValue::ExternalLinkage, "PrintParamNum", M);
        this->PrintParamNum->setCallingConv(CallingConv::C);
    } else {
        errs() << " Error when creating : " << "PrintParamNum" << "\n";
    }

    // PrintUnimplParam
    // void PrintUnimplParam();
    this->PrintUnimplParam = M.getFunction("PrintUnimplParam");
    if (!this->PrintUnimplParam) {
        ArgTypes.clear();
        FunctionType *PrintUnimplParam_FuncTy = FunctionType::get(this->VoidType, ArgTypes, false);
        this->PrintUnimplParam = Function::Create(PrintUnimplParam_FuncTy, GlobalValue::ExternalLinkage,
                                                  "PrintUnimplParam", M);
        this->PrintUnimplParam->setCallingConv(CallingConv::C);
    } else {
        errs() << " Error when creating : " << "PrintUnimplParam" << "\n";
    }
}


void PAInstrumentor::print(raw_ostream &O, const Module *M) const {

}


bool PAInstrumentor::runOnModule(llvm::Module &M) {
    InitTypes(M);
    InitHooks(M);

    map<string, set<Function *> > mapGenericFunctions = collectGenericFuns(M);

    bool Instrumented = false;

    for (auto itMapBegin = mapGenericFunctions.begin(); itMapBegin != mapGenericFunctions.end(); itMapBegin++) {
        for (auto pFF = itMapBegin->second.begin(); pFF != itMapBegin->second.end(); pFF++) {
            Function *F = *pFF;
            IRBuilder<> Builder(&*F->getEntryBlock().getFirstInsertionPt());
            // instrument PrintFuncName
            {
                Constant *FuncName = Builder.CreateGlobalStringPtr(itMapBegin->first);
                Builder.CreateCall(
                        PrintFuncName, {FuncName});
            }

            for (auto arg = F->arg_begin(); arg != F->arg_end(); arg++) {
                auto type = arg->getType();
                Builder.CreateCall(
                        PrintParamNum, ConstantInt::get(this->IntType, arg->getArgNo()));
                if (type->isIntegerTy()) {
                    Builder.CreateCall(
                            PrintIntParam, {dyn_cast<Value>(arg)});
                } else {
                    Builder.CreateCall(
                            PrintUnimplParam, None);
                }
            }
            Instrumented = true;
        }
    }

    return Instrumented;
}


