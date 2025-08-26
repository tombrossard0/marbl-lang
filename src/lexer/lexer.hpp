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
    inline static Lexer *activeLexer = nullptr;

    Lexer(std::istream &input, std::string filename) : scanner(&input), filename(filename) {
        activeLexer = this;
    }

    int nextToken();
    const std::string &getFilename() const { return filename; }

  private:
    yyFlexLexer scanner;
    std::string filename;
};
