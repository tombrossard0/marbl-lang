#include "llvm_codegen.hpp"

llvm::Value *CodeGenVisitor::convertToi1(llvm::Value *value) {
    if (value->getType()->isIntegerTy(32)) {
        return builder.CreateICmpNE(value, llvm::ConstantInt::get(context, llvm::APInt(32, 0)), "ifcond");
    } else if (value->getType()->isDoubleTy()) {
        return builder.CreateFCmpONE(value, llvm::ConstantFP::get(context, llvm::APFloat(0.0)), "ifcond");
    } else if (!value->getType()->isIntegerTy(1)) {
        throw std::runtime_error("Invalid if condition type");
    }

    return value;
}

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
            else if constexpr (std::is_same_v<T, struct Identifier>) {
                return env->get(val.id);
            } else {
                throw std::runtime_error("Type not yet supported in codegen");
            }
        },
        expr.value);
}

llvm::Value *CodeGenVisitor::visitBinaryExpr(Binary &expr) {
    auto *L = expr.left->accept(*this);
    auto *R = expr.right->accept(*this);

    switch (expr.op.type) {
    case TokenType::PLUS:
        if (L->getType()->isDoubleTy()) return builder.CreateFAdd(L, R, "addtmp");
        // if (L->getType()->isPointerTy()) return emitStringConcat(L, R);
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

    case TokenType::LESS:
        if (L->getType()->isDoubleTy()) return builder.CreateFCmpOLT(L, R, "lttmp");
        return builder.CreateICmpSLT(L, R, "lttmp");

    case TokenType::LESS_EQUAL:
        if (L->getType()->isDoubleTy()) return builder.CreateFCmpOLE(L, R, "letmp");
        return builder.CreateICmpSLE(L, R, "letmp");

    case TokenType::GREATER:
        if (L->getType()->isDoubleTy()) return builder.CreateFCmpOGT(L, R, "gttmp");
        return builder.CreateICmpSGT(L, R, "gttmp");

    case TokenType::GREATER_EQUAL:
        if (L->getType()->isDoubleTy()) return builder.CreateFCmpOGE(L, R, "getmp");
        return builder.CreateICmpSGE(L, R, "gttmp");

    case TokenType::EQUAL_EQUAL:
        if (L->getType()->isDoubleTy()) return builder.CreateFCmpOEQ(L, R, "eqtmp");
        return builder.CreateICmpEQ(L, R, "eqtmp");

    case TokenType::BANG_EQUAL:
        if (L->getType()->isDoubleTy()) return builder.CreateFCmpONE(L, R, "netmp");
        return builder.CreateICmpNE(L, R, "netmp");

    default:
        throw std::runtime_error("Unsupported binary operator");
    }
}

llvm::Value *CodeGenVisitor::visitLogicalExpr(Logical &expr) {
    auto *L = convertToi1(expr.left->accept(*this));
    auto *R = convertToi1(expr.right->accept(*this));

    switch (expr.op.type) {
    case TokenType::AND:
        return builder.CreateLogicalAnd(L, R, "andtmp");

    case TokenType::OR:
        return builder.CreateLogicalOr(L, R, "ortmp");

    default:
        throw std::runtime_error("Unsupported logical operator");
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

llvm::Value *CodeGenVisitor::visitVariableExpr(Variable &expr) {
    llvm::Value *alloca = env->get(expr.name.lexeme);
    auto *ptrTy = llvm::cast<llvm::AllocaInst>(alloca)->getAllocatedType();
    return builder.CreateLoad(ptrTy, alloca, expr.name.lexeme);
}

llvm::Value *CodeGenVisitor::visitAssignExpr(Assign &expr) {
    llvm::Value *value = expr.value->accept(*this);
    env->assign(*this, expr.name.lexeme, value);
    return value;
}

void CodeGenVisitor::visitExpressionStmt(Expression &stmt) {
    auto *res = stmt.expression->accept(*this);
}

void CodeGenVisitor::visitPrintStmt(Print &stmt) {
    auto *res = stmt.expression->accept(*this);

    llvm::Value *formatStr = nullptr;
    if (res->getType()->isIntegerTy(32))
        formatStr = builder.CreateGlobalStringPtr("%d\n");
    else if (res->getType()->isIntegerTy(1))
        formatStr = builder.CreateGlobalStringPtr("%d\n");
    else if (res->getType()->isDoubleTy())
        formatStr = builder.CreateGlobalStringPtr("%f\n");
    else if (res->getType()->isPointerTy())
        formatStr = builder.CreateGlobalStringPtr("%s\n");
    else { throw std::runtime_error("Unsupported type for printing ;-;"); }

    llvm::FunctionType *printfType =
        llvm::FunctionType::get(builder.getInt32Ty(), llvm::PointerType::get(builder.getInt8Ty(), 0), true);
    llvm::FunctionCallee printfFunc = module.getOrInsertFunction("printf", printfType);
    builder.CreateCall(printfFunc, {formatStr, res});
}

void CodeGenVisitor::visitIfStmt(If &stmt) {
    llvm::Value *condValue = stmt.condition->accept(*this);

    // Convert to i1 (bool) if possible
    condValue = convertToi1(condValue);

    llvm::Function *function = builder.GetInsertBlock()->getParent();

    // Create blocks with parent function
    llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(context, "then", function);
    llvm::BasicBlock *elseBB =
        stmt.elseBranch ? llvm::BasicBlock::Create(context, "else", function) : nullptr;
    llvm::BasicBlock *endBB = llvm::BasicBlock::Create(context, "end", function);

    // Conditional branch
    if (elseBB)
        builder.CreateCondBr(condValue, thenBB, elseBB);
    else
        builder.CreateCondBr(condValue, thenBB, endBB);

    // Emit "then"
    builder.SetInsertPoint(thenBB);
    stmt.thenBranch->accept(*this);
    if (!builder.GetInsertBlock()->getTerminator())
        builder.CreateBr(
            endBB); // If the block does not already end with a terminator, insert a branch to endBB

    // Emit "else"
    if (stmt.elseBranch) {
        builder.SetInsertPoint(elseBB);
        stmt.elseBranch->accept(*this);
        if (!builder.GetInsertBlock()->getTerminator()) builder.CreateBr(endBB); // Idem
    }

    // End block
    builder.SetInsertPoint(endBB);
}

void CodeGenVisitor::visitLetStmt(Let &stmt) {
    llvm::Value *value = stmt.initializer->accept(*this);
    env->declare(*this, stmt.name.lexeme, value);
}

void CodeGenVisitor::visitBlockStmt(Block &stmt) {
    std::cout << "Enter block\n";
    std::unique_ptr<Environment> previousEnv = std::move(env);
    env = std::make_unique<Environment>(previousEnv.get());
    for (auto &subStmt : stmt.statements) { subStmt->accept(*this); }
    env = std::move(previousEnv);
    std::cout << "Leave block\n";
}

// === Entry point: wraps expression in function main ===
void CodeGenVisitor::generate(std::vector<UniqueStmt> &statements) {
    auto *funcType = llvm::FunctionType::get(builder.getInt32Ty(), false);
    auto *function = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);
    auto *bb = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(bb);

    for (auto &statement : statements) { statement->accept(*this); }

    builder.CreateRet(llvm::ConstantInt::get(context, llvm::APInt(32, 0)));
}
