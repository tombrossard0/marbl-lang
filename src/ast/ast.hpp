#pragma once

#include <memory>
#include <variant>

#include "tokens.hpp"

#define FIELD_MEMBER(type, name) type name;

#define FIELD_PARAMS(type, name) type name,
#define FIELD_PARAMS_END(type, name) type name

#define FIELD_INIT(type, name) name(std::move(name)),
#define FIELD_INIT_END(type, name) name(std::move(name))

#define DEFINE_AST(name, FIELDS)                                                                             \
    class name : public Expr {                                                                               \
      public:                                                                                                \
        FIELDS(FIELD_MEMBER, FIELD_MEMBER)                                                                   \
        name(FIELDS(FIELD_PARAMS, FIELD_PARAMS_END)) : FIELDS(FIELD_INIT, FIELD_INIT_END) {                  \
        }                                                                                                    \
    };

// === Definitions ===
using Object = std::variant<int, double, std::string, bool>;

class Expr {
  public:
    virtual ~Expr() = default;
};

using UniqueExpr = std::unique_ptr<Expr>;

// === Generated definitions ===
#define BINARY_FIELDS(X, Y) X(UniqueExpr, left) X(Token, op) Y(UniqueExpr, right) // Y = no comma at the end
DEFINE_AST(Binary, BINARY_FIELDS)

#define GROUPING_FIELDS(X, Y) Y(UniqueExpr, expression)
DEFINE_AST(Grouping, GROUPING_FIELDS)

#define LITERAL_FIELDS(X, Y) Y(Object, value)
DEFINE_AST(Literal, LITERAL_FIELDS)

#define UNARY_FIELDS(X, Y) X(UniqueExpr, op) Y(UniqueExpr, right)
DEFINE_AST(Unary, UNARY_FIELDS)
