#pragma once

#include "ast.hpp"

class AstPrinter : public Visitor<void> {
  public:
    void print(Expr &expr);

    void visitBinaryExpr(Binary &expr) override;

    void visitGroupingExpr(Grouping &expr) override;

    void visitLiteralExpr(Literal &expr) override;

    void visitUnaryExpr(Unary &expr) override;
};
