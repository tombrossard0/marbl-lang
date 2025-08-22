%option c++
%option noyywrap
%option outfile="lexer.yy.cpp"
%option header-file="lexer.yy.hpp"

%{
#include "lexer.hpp"
#include <iostream>

#define REPLACE(TYPE, LEXEME, LITERAL, LINE, LEN)\
    Lexer::currentToken.type = TYPE;\
    Lexer::currentToken.lexeme = LEXEME;\
    Lexer::currentToken.literal = LITERAL;\
    Lexer::currentToken.line = LINE;\
    Lexer::currentToken.col = current_col;\
    current_col += LEN;\
    return TYPE;

int current_line = 1;
int current_col = 1;
%}

DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT})*

%%

"let"                       { REPLACE(TokenType::LET, yytext, yytext, current_line, yyleng); }

[ \t]+                      { current_col += yyleng; } // skip whitespace
";"                         { REPLACE(TokenType::SEMICOLON, yytext, yytext, current_line, yyleng); }

\r\n                        { current_line++; current_col = 1; }
\r                          { current_line++; current_col = 1; }
\n                          { current_line++; current_col = 1; }

"<="                        { REPLACE(TokenType::LESS_EQUAL, yytext, yytext, current_line, yyleng); }
">="                        { REPLACE(TokenType::GREATER_EQUAL, yytext, yytext, current_line, yyleng); }
"<"                         { REPLACE(TokenType::LESS, yytext, yytext, current_line, yyleng); }
">"                         { REPLACE(TokenType::GREATER, yytext, yytext, current_line, yyleng); }
"=="                        { REPLACE(TokenType::EQUAL_EQUAL, yytext, yytext, current_line, yyleng); }
"="                         { REPLACE(TokenType::EQUAL, yytext, yytext, current_line, yyleng); }

{DIGIT}+\.?{DIGIT}*                    { REPLACE(TokenType::NUMBER, yytext, yytext, current_line, yyleng); }
{ID}                        { REPLACE(TokenType::IDENTIFIER, yytext, yytext, current_line, yyleng); }
\"(.)+\"                    { REPLACE(TokenType::STRING, yytext, std::string_view(yytext + 1, yyleng - 2), current_line, yyleng); }

<<EOF>>                     { REPLACE(TokenType::T_EOF, yytext, yytext, current_line, yyleng); }

.                           { REPLACE(TokenType::ERROR, yytext, yytext, current_line, yyleng); };

%%
