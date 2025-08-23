#include "interpreter.hpp"

#include <variant>

void Interpreter::interpret(Expr &expr) {
    try {
        Object res = evaluate(expr);
        std::cout << res << std::endl;
    } catch (std::runtime_error err) { std::cerr << "Runtime error: " << err.what() << std::endl; }
}

Object Interpreter::evaluate(Expr &expr) {
    return expr.accept(*this);
}

Object Interpreter::visitLiteralExpr(Literal &expr) {
    std::cout << "test literal: " << expr.value << std::endl;
    return expr.value;
}

Object Interpreter::visitBinaryExpr(Binary &expr) {
    Object left = evaluate(*expr.left);
    Object right = evaluate(*expr.right);

    switch (expr.op.type) {
    case PLUS:
        return std::visit(
            [](auto &&left, auto &&right) -> Object {
                using LEFT_TYPE = std::decay_t<decltype(left)>;
                using RIGHT_TYPE = std::decay_t<decltype(right)>;

                if constexpr (std::is_same_v<LEFT_TYPE, int> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left + right;
                else if constexpr (std::is_same_v<LEFT_TYPE, double> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left + right;
                else
                    throw std::runtime_error("Plus operation not valid for these types.");
            },
            left, right);
    case MINUS:
        return std::visit(
            [](auto &&left, auto &&right) -> Object {
                using LEFT_TYPE = std::decay_t<decltype(left)>;
                using RIGHT_TYPE = std::decay_t<decltype(right)>;

                if constexpr (std::is_same_v<LEFT_TYPE, int> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left - right;
                else if constexpr (std::is_same_v<LEFT_TYPE, double> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left - right;
                else
                    throw std::runtime_error("Minus operation not valid for these types.");
            },
            left, right);
    case STAR:
        return std::visit(
            [](auto &&left, auto &&right) -> Object {
                using LEFT_TYPE = std::decay_t<decltype(left)>;
                using RIGHT_TYPE = std::decay_t<decltype(right)>;

                if constexpr (std::is_same_v<LEFT_TYPE, int> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left * right;
                else if constexpr (std::is_same_v<LEFT_TYPE, double> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left * right;
                else
                    throw std::runtime_error("Star operation not valid for these types.");
            },
            left, right);
    case SLASH:
        return std::visit(
            [](auto &&left, auto &&right) -> Object {
                using LEFT_TYPE = std::decay_t<decltype(left)>;
                using RIGHT_TYPE = std::decay_t<decltype(right)>;

                if constexpr (std::is_same_v<LEFT_TYPE, int> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left / right;
                else if constexpr (std::is_same_v<LEFT_TYPE, double> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left / right;
                else
                    throw std::runtime_error("Slash operation not valid for these types.");
            },
            left, right);
    case GREATER:
        return std::visit(
            [](auto &&left, auto &&right) -> Object {
                using LEFT_TYPE = std::decay_t<decltype(left)>;
                using RIGHT_TYPE = std::decay_t<decltype(right)>;

                if constexpr (std::is_same_v<LEFT_TYPE, int> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left > right;
                else if constexpr (std::is_same_v<LEFT_TYPE, double> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left > right;
                else
                    throw std::runtime_error("Grater operation not valid for these types.");
            },
            left, right);
    case GREATER_EQUAL:
        return std::visit(
            [](auto &&left, auto &&right) -> Object {
                using LEFT_TYPE = std::decay_t<decltype(left)>;
                using RIGHT_TYPE = std::decay_t<decltype(right)>;

                if constexpr (std::is_same_v<LEFT_TYPE, int> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left >= right;
                else if constexpr (std::is_same_v<LEFT_TYPE, double> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left >= right;
                else
                    throw std::runtime_error("Greater Equal operation not valid for these types.");
            },
            left, right);
    case EQUAL_EQUAL:
        return std::visit(
            [](auto &&left, auto &&right) -> Object {
                using LEFT_TYPE = std::decay_t<decltype(left)>;
                using RIGHT_TYPE = std::decay_t<decltype(right)>;

                if constexpr (std::is_same_v<LEFT_TYPE, int> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left == right;
                else if constexpr (std::is_same_v<LEFT_TYPE, double> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left == right;
                else
                    return false;
            },
            left, right);
    case BANG_EQUAL:
        return std::visit(
            [](auto &&left, auto &&right) -> Object {
                using LEFT_TYPE = std::decay_t<decltype(left)>;
                using RIGHT_TYPE = std::decay_t<decltype(right)>;

                if constexpr (std::is_same_v<LEFT_TYPE, int> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left != right;
                else if constexpr (std::is_same_v<LEFT_TYPE, double> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left != right;
                else
                    return false;
            },
            left, right);
    case LESS:
        return std::visit(
            [](auto &&left, auto &&right) -> Object {
                using LEFT_TYPE = std::decay_t<decltype(left)>;
                using RIGHT_TYPE = std::decay_t<decltype(right)>;

                if constexpr (std::is_same_v<LEFT_TYPE, int> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left < right;
                else if constexpr (std::is_same_v<LEFT_TYPE, double> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left < right;
                else
                    throw std::runtime_error("Less operation not valid for these types.");
            },
            left, right);
    case LESS_EQUAL:
        return std::visit(
            [](auto &&left, auto &&right) -> Object {
                using LEFT_TYPE = std::decay_t<decltype(left)>;
                using RIGHT_TYPE = std::decay_t<decltype(right)>;

                if constexpr (std::is_same_v<LEFT_TYPE, int> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left <= right;
                else if constexpr (std::is_same_v<LEFT_TYPE, double> && std::is_same_v<LEFT_TYPE, RIGHT_TYPE>)
                    return left <= right;
                else
                    throw std::runtime_error("Less Equal operation not valid for these types.");
            },
            left, right);
    default:
        break;
    }

    throw std::runtime_error("This line should never be reached, it yes, please check the parser.");
}

Object Interpreter::visitGroupingExpr(Grouping &expr) {
    return evaluate(*expr.expression);
}

Object Interpreter::visitUnaryExpr(Unary &expr) {
    Object right = evaluate(*expr.right);

    switch (expr.op.type) {
    case BANG:
        return std::visit(
            [](auto &&val) -> Object {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, bool>) return !val;
                if constexpr (std::is_same_v<T, int>)
                    return !val;
                else if constexpr (std::is_same_v<T, double>)
                    return !val;
                else
                    throw std::runtime_error("Unary bang operation not valid for this type.");
            },
            right);
    case MINUS:
        return std::visit(
            [](auto &&val) -> Object {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, int>)
                    return -val;
                else if constexpr (std::is_same_v<T, double>)
                    return -val;
                else
                    throw std::runtime_error("Unary minus operation not valid for this type.");
            },
            right);
    default:
        break;
    }

    throw std::runtime_error("This line should never be reached, it yes, please check the parser.");
}
