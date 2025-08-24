#pragma once

#include "ast.hpp"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

class CodeGenVisitor : public ExprVisitor<llvm::Value *>, StmtVisitor<void> {
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

    void generate(std::vector<UniqueStmt> &statements);

    llvm::Value *visitLiteralExpr(Literal &expr) override;
    llvm::Value *visitBinaryExpr(Binary &expr) override;
    llvm::Value *visitUnaryExpr(Unary &expr) override;
    llvm::Value *visitGroupingExpr(Grouping &expr) override;
    llvm::Value *visitVariableExpr(Variable &expr) override;
    llvm::Value *visitAssignExpr(Assign &expr) override;

    void visitExpressionStmt(Expression &stmt) override;
    void visitPrintStmt(Print &stmt) override;
    void visitLetStmt(Let &stmt) override;
};
