#pragma once

#include "ast.hpp"

class Interpreter : public Visitor<Object> {
  public:
    void interpret(Expr &expr);

  private:
    Object evaluate(Expr &expr);

    Object visitBinaryExpr(Binary &expr) override;
    Object visitGroupingExpr(Grouping &expr) override;
    Object visitLiteralExpr(Literal &expr) override;
    Object visitUnaryExpr(Unary &expr) override;

    Object visitExpressionStmt(Expression &stmt) override;
    Object visitPrintStmt(Print &stmt) override;
};
