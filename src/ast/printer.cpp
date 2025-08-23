#include "printer.hpp"

void AstPrinter::print(Expr &expr) {
    std::cout << "expression: ";
    expr.accept(*this);
    std::cout << std::endl;
}

void AstPrinter::visitBinaryExpr(Binary &expr) {
    std::cout << "( ";

    expr.left->accept(*this);
    std::cout << " ";

    std::cout << expr.op.literal << " ";

    expr.right->accept(*this);
    std::cout << " )";
}

void AstPrinter::visitGroupingExpr(Grouping &expr) {
    std::cout << "( group ";
    expr.expression->accept(*this);
    std::cout << " )";
}

void AstPrinter::visitLiteralExpr(Literal &expr) {
    std::visit(
        [](auto &&val) {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, std::string>) {
                std::cout << "\"" << val << "\"";
            } else if constexpr (std::is_same_v<T, bool>) {
                std::cout << (val ? "true" : "false");
            } else {
                std::cout << val;
            }
        },
        expr.value);
}

void AstPrinter::visitUnaryExpr(Unary &expr) {
    std::cout << "( " << expr.op.literal << " ";
    expr.right->accept(*this);
    std::cout << " )";
}
