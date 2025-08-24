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
