%option c++
%option noyywrap
%option outfile="lexer.yy.cpp"
%option header-file="lexer.yy.hpp"

%{
#include "lexer.hpp"
#include <iostream>

std::string unescape(const std::string& str) {
    std::string result;
    result.reserve(str.size());

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\\' && i + 1 < str.size()) {
            char next = str[i + 1];
            switch (next) {
                case 'n': result.push_back('\n'); break;
                case 't': result.push_back('\t'); break;
                case 'r': result.push_back('\r'); break;
                case '"': result.push_back('"'); break;
                case '\\': result.push_back('\\'); break;
                default: result.push_back(next); break;
            }
            ++i; // skip the escaped char
        } else {
            result.push_back(str[i]);
        }
    }

    return result;
}

#define REPLACE(TYPE, LITERAL)\
    Lexer::currentToken.type = TYPE;\
    Lexer::currentToken.lexeme = yytext;\
    Lexer::currentToken.literal = LITERAL;\
    Lexer::currentToken.line = current_line;\
    Lexer::currentToken.col = current_col;\
    current_col += yyleng;\
    return TYPE;

int current_line = 1;
int current_col = 1;
%}

DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT})*

NUMBER      ({DIGIT}+\.?{DIGIT}*|{DIGIT}*\.{DIGIT}+)
STRING      \"([^"]|\\.)+\"

%%

"("                         { REPLACE(TokenType::LEFT_PAREN, yytext); }
")"                         { REPLACE(TokenType::RIGHT_PAREN, yytext); }

"{"                         { REPLACE(TokenType::LEFT_BRACE, yytext); }
"}"                         { REPLACE(TokenType::RIGHT_BRACE, yytext); }

","                         { REPLACE(TokenType::COMMA, yytext); }
"."                         { REPLACE(TokenType::DOT, yytext); }
":"                         { REPLACE(TokenType::COLON, yytext); }

"-"                         { REPLACE(TokenType::MINUS, yytext); }
"+"                         { REPLACE(TokenType::PLUS, yytext); }
"/"                         { REPLACE(TokenType::SLASH, yytext); }
"*"                         { REPLACE(TokenType::STAR, yytext); }

"!"                         { REPLACE(TokenType::BANG, yytext); }
"!="                        { REPLACE(TokenType::BANG_EQUAL, yytext); }
"<="                        { REPLACE(TokenType::LESS_EQUAL, yytext); }
">="                        { REPLACE(TokenType::GREATER_EQUAL, yytext); }
"<"                         { REPLACE(TokenType::LESS, yytext); }
">"                         { REPLACE(TokenType::GREATER, yytext); }
"=="                        { REPLACE(TokenType::EQUAL_EQUAL, yytext); }
"="                         { REPLACE(TokenType::EQUAL, yytext); }

"-="                         { REPLACE(TokenType::MINUS_EQUAL, yytext); }
"+="                         { REPLACE(TokenType::PLUS_EQUAL, yytext); }
"/="                         { REPLACE(TokenType::SLASH_EQUAL, yytext); }
"*="                         { REPLACE(TokenType::STAR_EQUAL, yytext); }

"class"                     { REPLACE(TokenType::CLASS, yytext); }
"super"                     { REPLACE(TokenType::SUPER, yytext); }
"this"                      { REPLACE(TokenType::THIS, yytext); }

"fun"                       { REPLACE(TokenType::FUN, yytext); }
"return"                    { REPLACE(TokenType::RETURN, yytext); }

"if"                        { REPLACE(TokenType::IF, yytext); }
"else"                      { REPLACE(TokenType::ELSE, yytext); }
"for"                       { REPLACE(TokenType::FOR, yytext); }
"while"                     { REPLACE(TokenType::WHILE, yytext); }

"and"                       { REPLACE(TokenType::AND, yytext); }
"or"                        { REPLACE(TokenType::OR, yytext); }

"false"                     { REPLACE(TokenType::FALSE, false); }
"true"                      { REPLACE(TokenType::TRUE, true); }

"let"                       { REPLACE(TokenType::LET, yytext); }

[ \t]+                      { current_col += yyleng; } // skip whitespace
";"                         { REPLACE(TokenType::SEMICOLON, yytext); }
(\r\n|\r|\n)                { current_line++; current_col = 1; }

{NUMBER}                    {
                                std::string numStr(yytext);
                                if (numStr.find('.') != std::string::npos){
                                    REPLACE(TokenType::NUMBER, std::stod(numStr));
                                } else {
                                    REPLACE(TokenType::NUMBER, std::stoi(numStr));
                                }
                            }
{ID}                        { REPLACE(TokenType::IDENTIFIER, yytext); }
{STRING}                    { REPLACE(TokenType::STRING, unescape(std::string(yytext + 1, yyleng - 2))); }

<<EOF>>                     { REPLACE(TokenType::T_EOF, yytext); }

.                           { REPLACE(TokenType::ERROR, yytext); };

%%
