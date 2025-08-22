#include "marbl.hpp"

#include <vector>

#include "lexer.hpp"
#include "tokens.hpp"

int Marbl::runFile(char *filepath) {
    std::ifstream inputFile(filepath);

    if (!inputFile) {
        std::cerr << "Cannot open input file!" << std::endl;
        return EX_NOINPUT;
    }

    Lexer lexer(inputFile);
    int currentType;
    while ((currentType = lexer.nextToken()) != TokenType::T_EOF) {
        std::cout << lexer.currentToken << std::endl;
    }

    inputFile.close();

    if (hadError) EX_DATAERR;

    return EX_OK;
}

int Marbl::runPrompt() {
    int currentType = TokenType::T_SOF;

    do {
        Lexer lexer(std::cin);

        std::cout << "> ";
        while ((currentType = lexer.nextToken()) != TokenType::SEMICOLON && currentType != TokenType::T_EOF) {
            std::cout << lexer.currentToken << std::endl;
        }
        std::cout << lexer.currentToken << std::endl;
    } while (currentType != TokenType::T_EOF);

    if (hadError) EX_DATAERR;

    return EX_OK;
}
