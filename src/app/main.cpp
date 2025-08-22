#include "errors.hpp"
#include "lexer.hpp"

// We follow the conventions defined in UNIX "sysexits.h" header for exit codes:
// (https://man.freebsd.org/cgi/man.cgi?query=sysexits&apropos=0&sektion=0&manpath=FreeBSD+4.3-RELEASE&format=html).
int main(int argc, char **argv) {
    if (argc > 2) {
        std::cerr << "Usage: marbl [script]" << std::endl;
        return EX_USAGE;
    };

    Marbl::Lexer::hello_lexer();
    return 0;
}
