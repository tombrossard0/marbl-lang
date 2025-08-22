#include <fstream>
#include <iostream>

#include "errors.hpp"
#include "lexer.hpp"

namespace Marbl {
int runFile(char *filepath) {
    std::ifstream inputFile(filepath);

    if (!inputFile) {
        std::cerr << "Cannot open input file!" << std::endl;
        return EX_NOINPUT;
    }

    std::string line;
    while (std::getline(inputFile, line)) { std::cout << line << std::endl; }

    inputFile.close();

    return EX_OK;
}

int runPrompt() {
    std::string line;

    do {
        std::cout << "> ";
        std::getline(std::cin, line);
        std::cout << line << std::endl;
    } while (line != "exit");

    return EX_OK;
}
} // namespace Marbl

// We follow the conventions defined in UNIX "sysexits.h" header for exit codes:
// (https://man.freebsd.org/cgi/man.cgi?query=sysexits&apropos=0&sektion=0&manpath=FreeBSD+4.3-RELEASE&format=html).
int main(int argc, char **argv) {
    if (argc > 2) {
        std::cerr << "Usage: marbl [script]" << std::endl;
        return EX_USAGE;
    } else if (argc == 2) {
        return Marbl::runFile(argv[1]);
    } else {
        return Marbl::runPrompt();
    }

    Marbl::Lexer::hello_lexer();
    return EX_OK;
}
