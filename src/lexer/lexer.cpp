#include "lexer.hpp"

#include <sstream>

int Lexer::nextToken() {
    return scanner.yylex();
}
