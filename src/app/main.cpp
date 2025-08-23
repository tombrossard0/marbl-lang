#include <iostream>

#include "ast.hpp"
#include "marbl.hpp"
#include "printer.hpp"

// We follow the conventions defined in UNIX "sysexits.h" header for exit codes:
// (https://man.freebsd.org/cgi/man.cgi?query=sysexits&apropos=0&sektion=0&manpath=FreeBSD+4.3-RELEASE&format=html).
int main(int argc, char **argv) {
    std::unique_ptr<Binary> expression = std::make_unique<Binary>(
        std::make_unique<Unary>(Token(TokenType::MINUS, "-", "-", 1, 1), std::make_unique<Literal>(true)),
        Token(TokenType::STAR, "*", "*", 1, 1),
        std::make_unique<Grouping>(std::make_unique<Literal>("45.57")));

    AstPrinter printer;
    printer.print(*expression);

    if (argc > 2) {
        std::cerr << "Usage: marbl [script]" << std::endl;
        return EX_USAGE;
    }

    if (argc == 2) { return Marbl::runFile(argv[1]); }

    return Marbl::runPrompt();
}
