#include <fstream>
#include <iostream>

#include "errors.hpp"
#include "lexer.hpp"
#include "marbl.hpp"

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

    Lexer::hello_lexer();
    return EX_OK;
}
