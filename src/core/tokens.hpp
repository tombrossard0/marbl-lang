#pragma once

#include <iostream>

enum TokenType {
    // Single-character tokens.
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens.
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals.
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords.
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    LET,
    WHILE,

    T_EOF,

    ERROR = -1,
};

inline const char *TokenTypeName(TokenType type) {
    switch (type) {
    case LEFT_PAREN:
        return "LEFT_PAREN";
    case RIGHT_PAREN:
        return "RIGHT_PAREN";
    case LEFT_BRACE:
        return "LEFT_BRACE";
    case RIGHT_BRACE:
        return "RIGHT_BRACE";
    case COMMA:
        return "COMMA";
    case DOT:
        return "DOT";
    case MINUS:
        return "MINUS";
    case PLUS:
        return "PLUS";
    case SEMICOLON:
        return "SEMICOLON";
    case SLASH:
        return "SLASH";
    case STAR:
        return "STAR";
    case BANG:
        return "BANG";
    case BANG_EQUAL:
        return "BANG_EQUAL";
    case EQUAL:
        return "EQUAL";
    case EQUAL_EQUAL:
        return "EQUAL_EQUAL";
    case GREATER:
        return "GREATER";
    case GREATER_EQUAL:
        return "GREATER_EQUAL";
    case LESS:
        return "LESS";
    case LESS_EQUAL:
        return "LESS_EQUAL";
    case IDENTIFIER:
        return "IDENTIFIER";
    case STRING:
        return "STRING";
    case NUMBER:
        return "NUMBER";
    case AND:
        return "AND";
    case CLASS:
        return "CLASS";
    case ELSE:
        return "ELSE";
    case FALSE:
        return "FALSE";
    case FUN:
        return "FUN";
    case FOR:
        return "FOR";
    case IF:
        return "IF";
    case NIL:
        return "NIL";
    case OR:
        return "OR";
    case PRINT:
        return "PRINT";
    case RETURN:
        return "RETURN";
    case SUPER:
        return "SUPER";
    case THIS:
        return "THIS";
    case TRUE:
        return "TRUE";
    case LET:
        return "LET";
    case WHILE:
        return "WHILE";
    case T_EOF:
        return "T_EOF";
    case ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

class Token {
  public:
    const TokenType type;
    const std::string lexeme;
    const std::string literal;
    const int line;

    Token(TokenType type, std::string lexeme, std::string literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line) {}
    ~Token() {};

    inline friend std::ostream &operator<<(std::ostream &os, const Token &t) {
        os << TokenTypeName(t.type) << " " << t.lexeme << " " << t.literal;
        return os;
    };
};
