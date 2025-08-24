#pragma once

#include <memory>
#include <string>

#include "llvm-18/llvm/IR/IRBuilder.h"
#include "tokens.hpp"

// ======= Utility Types =======
using UniqueExpr = std::unique_ptr<class Expr>;
using UniqueStmt = std::unique_ptr<class Stmt>;

// ======= AST Node Field Macros =======
#define FIELD_MEMBER(type, name) type name;
#define FIELD_PARAMS(type, name) type name,
#define FIELD_PARAMS_END(type, name) type name
#define FIELD_INIT(type, name) name(std::move(name)),
#define FIELD_INIT_END(type, name) name(std::move(name))

// ======= Forward Declarations of Expressions =======
class Expr;
class Binary;
class Grouping;
class Literal;
class Unary;
class Variable;
class Assign;

class Expression;
class Print;
class Let;

#define BINARY_FIELDS(X, Y) X(UniqueExpr, left) X(Token, op) Y(UniqueExpr, right)
#define GROUPING_FIELDS(X, Y) Y(UniqueExpr, expression)
#define LITERAL_FIELDS(X, Y) Y(Object, value)
#define UNARY_FIELDS(X, Y) X(Token, op) Y(UniqueExpr, right)
#define VARIABLE_FIELDS(X, Y) Y(Token, name)
#define ASSIGN_FIELDS(X, Y) X(Token, name) Y(UniqueExpr, value)

#define EXPRESSION_FIELDS(X, Y) Y(UniqueExpr, expression)
#define PRINT_FIELDS(X, Y) Y(UniqueExpr, expression)
#define LET_FIELDS(X, Y) X(Token, name) Y(UniqueExpr, initializer)

#define EXPR_AST_NODES(X)                                                                                    \
    X(Binary, BINARY_FIELDS, Expr)                                                                           \
    X(Grouping, GROUPING_FIELDS, Expr)                                                                       \
    X(Literal, LITERAL_FIELDS, Expr)                                                                         \
    X(Unary, UNARY_FIELDS, Expr)                                                                             \
    X(Variable, VARIABLE_FIELDS, Expr)                                                                       \
    X(Assign, ASSIGN_FIELDS, Expr)

#define STMT_AST_NODES(X)                                                                                    \
    X(Expression, EXPRESSION_FIELDS, Stmt)                                                                   \
    X(Print, PRINT_FIELDS, Stmt)                                                                             \
    X(Let, LET_FIELDS, Stmt)

// ======= Visitor Template =======
template <typename T> class ExprVisitor {
  public:
#define VISIT_METHOD(name, FIELDS, basename) virtual T visit##name##Expr(name &expr) = 0;
    EXPR_AST_NODES(VISIT_METHOD)
#undef VISIT_METHOD
};

template <typename T> class StmtVisitor {
  public:
#define VISIT_METHOD(name, FIELDS, basename) virtual T visit##name##Stmt(name &stmt) = 0;
    STMT_AST_NODES(VISIT_METHOD)
#undef VISIT_METHOD
};

// ======= Expr & Smt Base Class =======
class Expr {
  public:
    virtual ~Expr() = default;
    virtual void accept(ExprVisitor<void> &visitor) = 0;
    virtual Object accept(ExprVisitor<Object> &visitor) = 0;
    virtual llvm::Value *accept(ExprVisitor<llvm::Value *> &visitor) = 0;
};

class Stmt {
  public:
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor<void> &visitor) = 0;
    virtual Object accept(StmtVisitor<Object> &visitor) = 0;
    virtual llvm::Value *accept(StmtVisitor<llvm::Value *> &visitor) = 0;
};

// ======= AST Subclass Macro =======
#define DEFINE_AST_SUBCLASS(name, FIELDS, basename)                                                          \
    class name : public basename {                                                                           \
      public:                                                                                                \
        FIELDS(FIELD_MEMBER, FIELD_MEMBER)                                                                   \
        name(FIELDS(FIELD_PARAMS, FIELD_PARAMS_END)) : FIELDS(FIELD_INIT, FIELD_INIT_END) {                  \
        }                                                                                                    \
        void accept(basename##Visitor<void> &visitor) override {                                             \
            visitor.visit##name##basename(*this);                                                            \
        }                                                                                                    \
        Object accept(basename##Visitor<Object> &visitor) override {                                         \
            return visitor.visit##name##basename(*this);                                                     \
        }                                                                                                    \
        llvm::Value *accept(basename##Visitor<llvm::Value *> &visitor) override {                            \
            return visitor.visit##name##basename(*this);                                                     \
        }                                                                                                    \
    };

// ======= Generate AST Nodes =======
EXPR_AST_NODES(DEFINE_AST_SUBCLASS)
STMT_AST_NODES(DEFINE_AST_SUBCLASS)
