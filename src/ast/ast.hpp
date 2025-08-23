#pragma once

#include "tokens.hpp"
#include <memory>
#include <string>
#include <variant>

// ======= Utility Types =======
using Object = std::variant<int, double, std::string, bool>;
using UniqueExpr = std::unique_ptr<class Expr>;

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

#define BINARY_FIELDS(X, Y) X(UniqueExpr, left) X(Token, op) Y(UniqueExpr, right)
#define GROUPING_FIELDS(X, Y) Y(UniqueExpr, expression)
#define LITERAL_FIELDS(X, Y) Y(Object, value)
#define UNARY_FIELDS(X, Y) X(Token, op) Y(UniqueExpr, right)

#define AST_NODES(X)                                                                                         \
    X(Binary, BINARY_FIELDS)                                                                                 \
    X(Grouping, GROUPING_FIELDS)                                                                             \
    X(Literal, LITERAL_FIELDS)                                                                               \
    X(Unary, UNARY_FIELDS)

// ======= Visitor Template =======
template <typename T> class Visitor {
  public:
#define VISIT_METHOD(name, FIELDS) virtual T visit##name##Expr(name &expr) = 0;
    AST_NODES(VISIT_METHOD)
#undef VISIT_METHOD
};

// ======= Expr Base Class =======
class Expr {
  public:
    virtual ~Expr() = default;
    virtual void accept(Visitor<void> &visitor) = 0;
    virtual std::string accept(Visitor<std::string> &visitor) = 0;
};

// ======= AST Subclass Macro =======
#define DEFINE_AST_SUBCLASS(name, FIELDS)                                                                    \
    class name : public Expr {                                                                               \
      public:                                                                                                \
        FIELDS(FIELD_MEMBER, FIELD_MEMBER)                                                                   \
        name(FIELDS(FIELD_PARAMS, FIELD_PARAMS_END)) : FIELDS(FIELD_INIT, FIELD_INIT_END) {                  \
        }                                                                                                    \
        void accept(Visitor<void> &visitor) override {                                                       \
            visitor.visit##name##Expr(*this);                                                                \
        }                                                                                                    \
        std::string accept(Visitor<std::string> &visitor) override {                                         \
            return visitor.visit##name##Expr(*this);                                                         \
        }                                                                                                    \
    };

// ======= Generate AST Nodes =======
AST_NODES(DEFINE_AST_SUBCLASS)
