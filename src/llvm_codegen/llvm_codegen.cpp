#include "llvm_codegen.hpp"

llvm::Value *CodeGenVisitor::visitLiteralExpr(Literal &expr) {
    return std::visit(
        [&](auto &&val) -> llvm::Value * {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, int>)
                return llvm::ConstantInt::get(context, llvm::APInt(32, val));
            else if constexpr (std::is_same_v<T, double>)
                return llvm::ConstantFP::get(context, llvm::APFloat(val));
            else if constexpr (std::is_same_v<T, bool>)
                return llvm::ConstantInt::get(context, llvm::APInt(1, val));
            else if constexpr (std::is_same_v<T, std::string>)
                return builder.CreateGlobalStringPtr(val);
            else { throw std::runtime_error("Type not yet supported in codegen"); }
        },
        expr.value);
}

llvm::Value *CodeGenVisitor::visitBinaryExpr(Binary &expr) {
    auto *L = expr.left->accept(*this);
    auto *R = expr.right->accept(*this);

    switch (expr.op.type) {
    case TokenType::PLUS:
        if (L->getType()->isDoubleTy()) return builder.CreateFAdd(L, R, "addtmp");
        return builder.CreateAdd(L, R, "addtmp");

    case TokenType::MINUS:
        if (L->getType()->isDoubleTy()) return builder.CreateFSub(L, R, "subtmp");
        return builder.CreateSub(L, R, "subtmp");

    case TokenType::STAR:
        if (L->getType()->isDoubleTy()) return builder.CreateFMul(L, R, "multmp");
        return builder.CreateMul(L, R, "multmp");

    case TokenType::SLASH:
        if (L->getType()->isDoubleTy()) return builder.CreateFDiv(L, R, "divtmp");
        return builder.CreateSDiv(L, R, "divtmp");

    default:
        throw std::runtime_error("Unsupported binary operator");
    }
}

llvm::Value *CodeGenVisitor::visitUnaryExpr(Unary &expr) {
    auto *R = expr.right->accept(*this);
    switch (expr.op.type) {
    case TokenType::MINUS:
        if (R->getType()->isDoubleTy()) return builder.CreateFNeg(R, "negtmp");
        return builder.CreateNeg(R, "negtmp");
    case TokenType::BANG:
        return builder.CreateNot(R, "nottmp");
    default:
        throw std::runtime_error("Unsupported unary operator");
    }
}

llvm::Value *CodeGenVisitor::visitGroupingExpr(Grouping &expr) {
    return expr.expression->accept(*this);
}

// === Entry point: wraps expression in function main ===
void CodeGenVisitor::generate(Expr &expr) {
    auto *funcType = llvm::FunctionType::get(builder.getInt32Ty(), false);
    auto *function = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);
    auto *bb = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(bb);

    auto *res = expr.accept(*this);

    llvm::Value *formatStr = nullptr;
    if (res->getType()->isIntegerTy(32))
        formatStr = builder.CreateGlobalStringPtr("%d\n");
    else if (res->getType()->isIntegerTy(1))
        formatStr = builder.CreateGlobalStringPtr("%d\n");
    else if (res->getType()->isDoubleTy())
        formatStr = builder.CreateGlobalStringPtr("%f\n");
    else if (res->getType()->isPointerTy())
        formatStr = builder.CreateGlobalStringPtr("%s\n");
    else
        throw std::runtime_error("Unsupported type for printing");

    llvm::FunctionType *printfType =
        llvm::FunctionType::get(builder.getInt32Ty(), llvm::PointerType::get(builder.getInt8Ty(), 0), true);
    llvm::FunctionCallee printfFunc = module.getOrInsertFunction("printf", printfType);
    builder.CreateCall(printfFunc, {formatStr, res});

    builder.CreateRet(llvm::ConstantInt::get(context, llvm::APInt(32, 0)));
}
