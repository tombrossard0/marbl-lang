#include "printer.hpp"

void AstPrinter::print(Expr &expr) {
    std::cout << "expression: ";
    expr.accept(*this);
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

void AstPrinter::visitExpressionStmt(Expression &stmt) {
}

void AstPrinter::visitPrintStmt(Print &stmt) {
}
