#include "lexer.hpp"

#include <iostream>

#include "utils.hpp"

namespace Marbl::Lexer {
void hello_lexer() {
    std::cout << Utils::trim("     Hello Lexer!    ") << std::endl;
}
} // namespace Marbl::Lexer
