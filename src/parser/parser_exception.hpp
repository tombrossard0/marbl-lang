#pragma once

#include <iostream>
#include <string>

#include "tokens.hpp"

class ParserException : public std::exception {
  public:
    ParserException(Token tok, const std::string &msg) : tok(tok), msg(msg) {
        if (tok.type == TokenType::T_EOF) {
            report(tok, "at end", msg);
        } else {
            report(tok, "at '" + tok.lexeme + "'", msg);
        }
        std::cerr << "Error with token: " << tok << std::endl;
    }
    ~ParserException() {};

    virtual const char *what() const throw() { return msg.c_str(); }

  private:
    const std::string msg;
    Token tok;

    void report(const Token tok, const std::string &at, const std::string &msg) {
        std::cerr << tok.filename << ":" << tok.line << ":" << tok.col << " " << at << ": error: " << msg
                  << std::endl;
    }
};
