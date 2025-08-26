#include "marbl.hpp"

#include <sstream>
#include <vector>

#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "printer.hpp"
#include "tokens.hpp"

int Marbl::runFile(char *filepath) {
    std::ifstream inputFile(filepath);

    if (!inputFile) {
        std::cerr << "Cannot open input file!" << std::endl;
        return EX_NOINPUT;
    }

    Parser parser{inputFile, std::string(filepath)};
    std::vector<UniqueStmt> statements = parser.parse();

    for (auto &statement : statements) {
        AstPrinter printer{};
        printer.print(*statement);

        // Interpreter interpreter{};
        // interpreter.interpret(*statement);
    }

    inputFile.close();

    if (hadError) EX_DATAERR;

    return EX_OK;
}

int Marbl::runPrompt() {
    std::string line;

    // while (true) {
    //     std::cout << "> ";
    //     if (!std::getline(std::cin, line)) break; // EOF

    //     if (line.empty()) continue;

    //     std::stringstream ss(line);
    //     Parser parser{ss};
    //     if (parser.lexer.currentToken.type == T_EOF) parser.lexer.nextToken(); // RESET

    //     while (!parser.isAtEnd()) {
    //         UniqueExpr expression;
    //         expression = parser.parse();
    //         if (!expression) {
    //             std::cerr << "Parse returned null expression." << std::endl;
    //             hadError = true;
    //             continue;
    //         }

    //         AstPrinter printer{};
    //         printer.print(*expression);

    //         Interpreter interpreter{};
    //         interpreter.interpret(*expression);
    //     }
    // }

    std::cout << std::endl;

    return hadError ? EX_DATAERR : EX_OK;
}
