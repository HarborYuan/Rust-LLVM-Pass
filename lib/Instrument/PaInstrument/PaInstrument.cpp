//
// Created by Haobo on 2020/7/17.
//

#include <string>
#include <map>
#include <set>
#include <iostream>

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
    this->ShortType = IntegerType::get(M.getContext(), 16);
    this->CharType = IntegerType::get(M.getContext(), 8);
    this->BoolType = IntegerType::get(M.getContext(), 1);

    this->VoidPointerType = PointerType::get(this->CharType, 0);
    this->CharPointerType = PointerType::get(this->CharType, 0);
    this->LongPointerType = PointerType::get(this->LongType, 0);

    this->DoubleType = Type::getDoubleTy(M.getContext());
    this->FloatType = Type::getFloatTy(M.getContext());
}

void PAInstrumentor::InitHooks(Module &M) {
    std::vector<Type *> ArgTypes;

    // PrintBackTrace
    // void PrintBackTrace();
    this->PrintBackTrace = M.getFunction("PrintBackTrace");
    if (!this->PrintBackTrace) {
        ArgTypes.clear();
        FunctionType *PrintBackTrace_FuncTy = FunctionType::get(this->VoidType, ArgTypes, false);
        this->PrintBackTrace = Function::Create(PrintBackTrace_FuncTy, GlobalValue::ExternalLinkage, "PrintBackTrace",
                                                M);
        this->PrintBackTrace->setCallingConv(CallingConv::C);
    } else {
        errs() << " Error when creating : " << "PrintBackTrace" << "\n";
    }


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


    // PrintType
    // void PrintType(char *i);
    this->PrintType = M.getFunction("PrintType");
    if (!this->PrintType) {
        ArgTypes.clear();
        ArgTypes.push_back(this->CharPointerType);
        FunctionType *PrintType_FuncTy = FunctionType::get(this->VoidType, ArgTypes, false);
        this->PrintType = Function::Create(PrintType_FuncTy, GlobalValue::ExternalLinkage, "PrintType", M);
        this->PrintType->setCallingConv(CallingConv::C);
    } else {
        errs() << " Error when creating : " << "PrintType" << "\n";
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

    // PrintLongParam
    // void PrintLongParam(long long i)
    this->PrintLongParam = M.getFunction("PrintLongParam");
    if (!this->PrintLongParam) {
        ArgTypes.clear();
        ArgTypes.push_back(this->LongType);
        FunctionType *PrintLongParam_FuncTy = FunctionType::get(this->VoidType, ArgTypes, false);
        this->PrintLongParam = Function::Create(PrintLongParam_FuncTy, GlobalValue::ExternalLinkage, "PrintLongParam",
                                                M);
        this->PrintLongParam->setCallingConv(CallingConv::C);
    } else {
        errs() << " Error when creating : " << "PrintLongParam" << "\n";
    }


    // PrintShortParam
    // void PrintShortParam(short i);
    this->PrintShortParam = M.getFunction("PrintShortParam");
    if (!this->PrintShortParam) {
        ArgTypes.clear();
        ArgTypes.push_back(this->ShortType);
        FunctionType *PrintShortParam_FuncTy = FunctionType::get(this->VoidType, ArgTypes, false);
        this->PrintShortParam = Function::Create(PrintShortParam_FuncTy, GlobalValue::ExternalLinkage,
                                                 "PrintShortParam", M);
        this->PrintShortParam->setCallingConv(CallingConv::C);
    } else {
        errs() << " Error when creating : " << "PrintShortParam" << "\n";
    }

    // PrintCharParam
    // void PrintCharParam(char i);
    this->PrintCharParam = M.getFunction("PrintCharParam");
    if (!this->PrintCharParam) {
        ArgTypes.clear();
        ArgTypes.push_back(this->CharType);
        FunctionType *PrintCharParam_FuncTy = FunctionType::get(this->VoidType, ArgTypes, false);
        this->PrintCharParam = Function::Create(PrintCharParam_FuncTy, GlobalValue::ExternalLinkage, "PrintCharParam",
                                                M);
        this->PrintCharParam->setCallingConv(CallingConv::C);
    } else {
        errs() << " Error when creating : " << "PrintCharParam" << "\n";
    }


    // PrintDoubleParam
    // void PrintDoubleParam(double i);
    this->PrintDoubleParam = M.getFunction("PrintDoubleType");
    if (!this->PrintDoubleParam) {
        ArgTypes.clear();
        ArgTypes.push_back(this->DoubleType);
        FunctionType *PrintDoubleParam_FuncTy = FunctionType::get(this->VoidType, ArgTypes, false);
        this->PrintDoubleParam = Function::Create(PrintDoubleParam_FuncTy, GlobalValue::ExternalLinkage, "PrintDoubleParam",
                                                M);
        this->PrintDoubleParam->setCallingConv(CallingConv::C);
    } else {
        errs() << " Error when creating : " << "PrintDoubleParam" << "\n";
    }


    // PrintFloatParam
    // void PrintFloatParam(double i);
    this->PrintFloatParam = M.getFunction("PrintFloatParam");
    if (!this->PrintFloatParam) {
        ArgTypes.clear();
        ArgTypes.push_back(this->FloatType);
        FunctionType *PrintFloatParam_FuncTy = FunctionType::get(this->VoidType, ArgTypes, false);
        this->PrintFloatParam = Function::Create(PrintFloatParam_FuncTy, GlobalValue::ExternalLinkage, "PrintFloatParam",
                                                  M);
        this->PrintFloatParam->setCallingConv(CallingConv::C);
    } else {
        errs() << " Error when creating : " << "PrintFloatParam" << "\n";
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
            // instrument PrintBackTrace
            {
//                Not working well currently
//                Builder.CreateCall(
//                        PrintBackTrace, None);
            }
            for (auto arg = F->arg_begin(); arg != F->arg_end(); arg++) {
                auto type = arg->getType();
                Builder.CreateCall(
                        PrintParamNum, ConstantInt::get(this->IntType, arg->getArgNo()));
                // Print type name
                {
                    std::string type_name;
                    raw_string_ostream oss(type_name);
                    oss << *type;
                    Constant *TypeName = Builder.CreateGlobalStringPtr(oss.str());
                    Builder.CreateCall(
                            PrintType, {TypeName});
                }

                if (type->isIntegerTy()) {
                    if (type->isIntegerTy(64)) {
                        Builder.CreateCall(
                                PrintLongParam, {dyn_cast<Value>(arg)});
                    } else if (type->isIntegerTy(32)) {
                        Builder.CreateCall(
                                PrintIntParam, {dyn_cast<Value>(arg)});
                    } else if (type->isIntegerTy(16)) {
                        Builder.CreateCall(
                                PrintShortParam, {dyn_cast<Value>(arg)});
                    } else if (type->isIntegerTy(8)) {
                        Builder.CreateCall(
                                PrintCharParam, {dyn_cast<Value>(arg)});
                    } else {
                        Builder.CreateCall(
                                PrintUnimplParam, None);
                    }
                } else if(type->isDoubleTy()) {
                    Builder.CreateCall(
                            PrintDoubleParam, {dyn_cast<Value>(arg)});
                }  else if (type->isFloatTy()) {
                    Builder.CreateCall(
                            PrintFloatParam, {dyn_cast<Value>(arg)});
                } else{
                    Builder.CreateCall(
                            PrintUnimplParam, None);
                }
            }
            Instrumented = true;
        }
    }

    return Instrumented;
}


