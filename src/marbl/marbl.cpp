#include "marbl.hpp"

#include <vector>

#include "lexer.hpp"
#include "tokens.hpp"

int Marbl::run(const std::string &source) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    for (Token token : tokens) { std::cout << token << std::endl; }

    if (hadError) EX_DATAERR;

    return EX_OK;
}

int Marbl::runFile(char *filepath) {
    std::ifstream inputFile(filepath);

    if (!inputFile) {
        std::cerr << "Cannot open input file!" << std::endl;
        return EX_NOINPUT;
    }

    std::string line;
    while (std::getline(inputFile, line) && !hadError) {
        std::cout << line << std::endl;
        run(line);
    }

    inputFile.close();

    return EX_OK;
}

int Marbl::runPrompt() {
    std::string line;

    bool isRunning;
    do {
        std::cout << "> ";
        isRunning = static_cast<bool>(std::getline(std::cin, line));
        std::cout << line << std::endl;

        run(line);
        hadError = false;
    } while (line != "exit" && isRunning);

    return EX_OK;
}
