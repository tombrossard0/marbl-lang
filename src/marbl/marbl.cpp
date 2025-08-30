#include "marbl.hpp"

#include <sstream>
#include <vector>

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
    }

    inputFile.close();

    if (hadError) EX_DATAERR;

    return EX_OK;
}

int Marbl::runPrompt() {
    std::string line;
    std::cout << std::endl;

    return hadError ? EX_DATAERR : EX_OK;
}
