#pragma once

#include "ast.hpp"

class AstPrinter : public ExprVisitor<void>, StmtVisitor<void> {
  public:
    void print(Stmt &expr);

  private:
    void visitBinaryExpr(Binary &expr) override;
    void visitLogicalExpr(Logical &expr) override;
    void visitGroupingExpr(Grouping &expr) override;
    void visitLiteralExpr(Literal &expr) override;
    void visitUnaryExpr(Unary &expr) override;
    void visitVariableExpr(Variable &expr) override;
    void visitAssignExpr(Assign &expr) override;

    void visitExpressionStmt(Expression &stmt) override;
    void visitPrintStmt(Print &stmt) override;
    void visitLetStmt(Let &stmt) override;
    void visitBlockStmt(Block &stmt) override;
    void visitIfStmt(If &stmt) override;
};
