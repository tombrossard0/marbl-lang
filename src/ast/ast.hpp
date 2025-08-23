#pragma once

#include <memory>
#include <variant>

#include "tokens.hpp"

#define FIELD_MEMBER(type, name) type name;

#define FIELD_PARAMS(type, name) type name,
#define FIELD_PARAMS_END(type, name) type name

#define FIELD_INIT(type, name) name(std::move(name)),
#define FIELD_INIT_END(type, name) name(std::move(name))

#define DEFINE_AST_SUBCLASS(name, FIELDS)                                                                    \
    class name : public Expr {                                                                               \
      public:                                                                                                \
        FIELDS(FIELD_MEMBER, FIELD_MEMBER)                                                                   \
        name(FIELDS(FIELD_PARAMS, FIELD_PARAMS_END)) : FIELDS(FIELD_INIT, FIELD_INIT_END) {                  \
        }                                                                                                    \
        template <typename T> T accept(class Visitor<T> &visitor) {                                          \
            return visitor.visit##name##Expr(*this);                                                         \
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
#define GROUPING_FIELDS(X, Y) Y(UniqueExpr, expression)
#define LITERAL_FIELDS(X, Y) Y(Object, value)
#define UNARY_FIELDS(X, Y) X(UniqueExpr, op) Y(UniqueExpr, right)

#define AST_NODES(X)                                                                                         \
    X(Binary, BINARY_FIELDS)                                                                                 \
    X(Grouping, GROUPING_FIELDS)                                                                             \
    X(Literal, LITERAL_FIELDS)                                                                               \
    X(Unary, UNARY_FIELDS)

template <typename T> class Visitor; // forward declaration

AST_NODES(DEFINE_AST_SUBCLASS)

template <typename T> class Visitor {
#define VISIT_METHOD(name, FIELDS) virtual T visit##name##Expr(name &expr) = 0;
    AST_NODES(VISIT_METHOD)
#undef VISIT_METHOD
};
