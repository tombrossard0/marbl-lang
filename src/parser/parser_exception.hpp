#pragma once

#include <iostream>
#include <string>

#include "tokens.hpp"

class ParserException : public std::exception {
  public:
    ParserException(Token tok, const std::string &msg) : tok(tok), msg(msg) {
        std::cerr << "Error with token: " << tok << std::endl;
        if (tok.type == TokenType::T_EOF) {
            report(tok.line, "at end", msg);
        } else {
            report(tok.line, "at '" + tok.lexeme + "'", msg);
        }
    }
    ~ParserException() {};

    virtual const char *what() const throw() { return msg.c_str(); }

  private:
    const std::string msg;
    Token tok;

    void report(const int line, const std::string &at, const std::string &msg) {
        std::cerr << "line " << line << " " << at << ". " << msg << std::endl;
    }
};
