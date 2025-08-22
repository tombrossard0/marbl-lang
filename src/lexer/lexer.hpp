#pragma once

#include <string>
#include <vector>

#include "tokens.hpp"

class Lexer {
  public:
    Lexer(const std::string &src) : source(src) {};

    std::vector<Token> scanTokens();

  private:
    std::string source;
};
