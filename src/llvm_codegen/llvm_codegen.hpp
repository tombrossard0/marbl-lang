#pragma once

#include "ast.hpp"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

class CodeGenVisitor : public Visitor<llvm::Value *> {
    llvm::LLVMContext context;
    llvm::Module module;
    llvm::IRBuilder<> builder;

    llvm::Function *strlenFunc;
    llvm::Function *mallocFunc;
    llvm::Function *memcpyFunc;

  public:
    CodeGenVisitor(const std::string &moduleName) : module(moduleName, context), builder(context) {
        // Declare external C functions
        strlenFunc = llvm::Function::Create(
            llvm::FunctionType::get(builder.getInt32Ty(), {builder.getIntPtrTy(module.getDataLayout(), 8)},
                                    false),
            llvm::Function::ExternalLinkage, "strlen", module);

        mallocFunc =
            llvm::Function::Create(llvm::FunctionType::get(builder.getIntPtrTy(module.getDataLayout(), 8),
                                                           {builder.getInt32Ty()}, false),
                                   llvm::Function::ExternalLinkage, "malloc", module);

        memcpyFunc = llvm::Function::Create(
            llvm::FunctionType::get(builder.getIntPtrTy(module.getDataLayout(), 8),
                                    {builder.getIntPtrTy(module.getDataLayout(), 8),
                                     builder.getIntPtrTy(module.getDataLayout(), 8), builder.getInt32Ty()},
                                    false),
            llvm::Function::ExternalLinkage, "memcpy", module);
    }

    llvm::Module &getModule() { return module; }
    void generate(Expr &expr);

    llvm::Value *visitLiteralExpr(Literal &expr) override;
    llvm::Value *visitBinaryExpr(Binary &expr) override;
    llvm::Value *visitUnaryExpr(Unary &expr) override;
    llvm::Value *visitGroupingExpr(Grouping &expr) override;

    llvm::Value *visitExpressionStmt(Expression &stmt) override;
    llvm::Value *visitPrintStmt(Print &stmt) override;
};
