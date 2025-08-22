#pragma once

#ifndef __FLEX_LEXER_H
    #include <FlexLexer.h>
#endif

#include <string>
#include <vector>

#include "tokens.hpp"

class Lexer {
  public:
    inline static Token currentToken{};

    Lexer(std::istream &input) : scanner(&input) {};

    int nextToken();

  private:
    yyFlexLexer scanner;
};
