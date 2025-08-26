#pragma once

#include "ast.hpp"

class Interpreter : public ExprVisitor<Object>, StmtVisitor<void> {
  public:
    void interpret(Expr &expr);

  private:
    Object evaluate(Expr &expr);

    Object visitBinaryExpr(Binary &expr) override;
    Object visitLogicalExpr(Logical &expr) override;
    Object visitGroupingExpr(Grouping &expr) override;
    Object visitLiteralExpr(Literal &expr) override;
    Object visitUnaryExpr(Unary &expr) override;
    Object visitVariableExpr(Variable &expr) override;
    Object visitAssignExpr(Assign &expr) override;

    void visitExpressionStmt(Expression &stmt) override;
    void visitPrintStmt(Print &stmt) override;
    void visitLetStmt(Let &stmt) override;
    void visitBlockStmt(Block &stmt) override;
    void visitIfStmt(If &stmt) override;
};
