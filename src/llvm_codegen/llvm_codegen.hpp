#pragma once

#include "ast.hpp"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

class CodeGenVisitor : public Visitor<llvm::Value *> {
    llvm::LLVMContext context;
    llvm::Module module;
    llvm::IRBuilder<> builder;

  public:
    CodeGenVisitor(const std::string &moduleName) : module(moduleName, context), builder(context) {}
    llvm::Module &getModule() { return module; }

    void generate(Expr &expr);

    llvm::Value *visitLiteralExpr(Literal &expr) override;
    llvm::Value *visitBinaryExpr(Binary &expr) override;
    llvm::Value *visitUnaryExpr(Unary &expr) override;
    llvm::Value *visitGroupingExpr(Grouping &expr) override;
};
