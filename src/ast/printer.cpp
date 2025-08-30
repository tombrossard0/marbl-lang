#include "printer.hpp"

void AstPrinter::print(Stmt &stmt) {
    std::cout << "stmt: ";
    stmt.accept(*this);
    std::cout << std::endl;
}

void AstPrinter::visitBinaryExpr(Binary &expr) {
    std::cout << "( ";

    expr.left->accept(*this);
    std::cout << " ";

    std::cout << expr.op.lexeme << " ";

    expr.right->accept(*this);
    std::cout << " )";
}

void AstPrinter::visitLogicalExpr(Logical &expr) {
    std::cout << "( ";

    expr.left->accept(*this);
    std::cout << " ";

    std::cout << expr.op.lexeme << " ";

    expr.right->accept(*this);
    std::cout << " )";
}

void AstPrinter::visitGroupingExpr(Grouping &expr) {
    std::cout << "( group ";
    expr.expression->accept(*this);
    std::cout << " )";
}

void AstPrinter::visitLiteralExpr(Literal &expr) {
    std::cout << expr.value;
}

void AstPrinter::visitUnaryExpr(Unary &expr) {
    std::cout << "( " << expr.op.lexeme << " ";
    expr.right->accept(*this);
    std::cout << " )";
}

void AstPrinter::visitVariableExpr(Variable &expr) {
    std::cout << expr.name.lexeme;
}

void AstPrinter::visitAssignExpr(Assign &expr) {
    std::cout << expr.name.lexeme;
    std::cout << " = ";
    expr.value->accept(*this);
}

void AstPrinter::visitExpressionStmt(Expression &stmt) {
    stmt.expression->accept(*this);
    std::cout << ";";
}

void AstPrinter::visitPrintStmt(Print &stmt) {
    std::cout << "print(";
    stmt.expression->accept(*this);
    std::cout << ");";
}

void AstPrinter::visitIfStmt(If &stmt) {
    std::cout << "if (";
    stmt.condition->accept(*this);
    std::cout << ") ";
    stmt.thenBranch->accept(*this);

    if (stmt.elseBranch) {
        std::cout << "else ";
        stmt.elseBranch->accept(*this);
    }
}

void AstPrinter::visitWhileStmt(While &stmt) {
    std::cout << "while (";
    stmt.condition->accept(*this);
    std::cout << ") ";
    stmt.body->accept(*this);
}

void AstPrinter::visitLetStmt(Let &stmt) {
    std::cout << "let " << stmt.name.literal << " = ";
    stmt.initializer->accept(*this);
    std::cout << ";";
}

void AstPrinter::visitBlockStmt(Block &stmt) {
    std::cout << "{" << std::endl;
    for (auto &sub_stmt : stmt.statements) {
        sub_stmt->accept(*this);
        std::cout << std::endl;
    }
    std::cout << "}";
}

void AstPrinter::visitCallExpr(Call &expr) {
    expr.callee->accept(*this);

    std::cout << "(";
    for (auto &arg : expr.arguments) { arg->accept(*this); }
    std::cout << ")";
}

void AstPrinter::visitFunctionStmt(Function &stmt) {
    std::cout << "fn " << stmt.name.lexeme << "(";
    int i = 0;
    for (auto &param : stmt.params) {
        std::cout << param.lexeme;
        if (++i < stmt.params.size()) { std::cout << ", "; }
    }
    std::cout << ") {";

    for (auto &fn_stmt : stmt.body) { fn_stmt->accept(*this); }

    std::cout << "}";
}