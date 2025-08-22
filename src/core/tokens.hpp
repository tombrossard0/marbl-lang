#pragma once

#include <iostream>

enum TokenType {
    ERROR = -1,

    // Single-character tokens
    LEFT_PAREN,
    RIGHT_PAREN,

    LEFT_BRACE,
    RIGHT_BRACE,

    COMMA,
    DOT,

    // Operators
    MINUS,
    PLUS,
    SLASH,
    STAR,

    // Comparators
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Keywords
    CLASS,
    SUPER,
    THIS,

    FUN,
    RETURN,
    AWAIT,

    IF,
    ELSE,
    FOR,
    WHILE,

    AND,
    OR,

    FALSE,
    TRUE,

    LET,

    // Literals
    NUMBER,
    IDENTIFIER,
    STRING,

    PRINT,

    // Sepatators
    SEMICOLON,
    NEWLINE,
    T_SOF, // Start of File
    T_EOF,
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
    case AWAIT:
        return "AWAIT";
    case FOR:
        return "FOR";
    case IF:
        return "IF";
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
    case NEWLINE:
        return "NEWLINE";
    case T_SOF:
        return "T_SOF";
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
    TokenType type;
    std::string lexeme;
    std::string literal;
    int line;
    int col;

    Token() : type(TokenType::T_SOF), lexeme("T_SOF"), literal("T_SOF"), line(0) {}
    Token(TokenType type, std::string lexeme, std::string literal, int line, int col)
        : type(type), lexeme(lexeme), literal(literal), line(line), col(col) {}
    ~Token() {};

    inline friend std::ostream &operator<<(std::ostream &os, const Token &t) {
        os << TokenTypeName(t.type) << " " << t.lexeme << " " << t.literal << " " << t.line << ":" << t.col;
        return os;
    };
};
