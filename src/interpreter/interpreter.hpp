#pragma once

#include "ast.hpp"

class interpreter : public Visitor<Object> {
    Object visitBinaryExpr(Binary &expr) override;
    Object visitGroupingExpr(Grouping &expr) override;
    Object visitLiteralExpr(Literal &expr) override;
    Object visitUnaryExpr(Unary &expr) override;
};
