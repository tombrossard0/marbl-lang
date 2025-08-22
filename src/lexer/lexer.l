%option c++
%option noyywrap
%option outfile="lexer.yy.cpp"
%option header-file="lexer.yy.hpp"

%{
#include "tokens.hpp"
%}

DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT})*

%%

[ \t\n]+                    ; // skip whitespace

"let"                       { return static_cast<int>(TokenType::LET); }

"="                         { return static_cast<int>(TokenType::EQUAL); }

";"                         { return static_cast<int>(TokenType::SEMICOLON); }

{DIGIT}+                    { return static_cast<int>(TokenType::NUMBER); }
{ID}                        { return static_cast<int>(TokenType::IDENTIFIER); }
"\""({LETTER}|{DIGIT})+"\"" { return static_cast<int>(TokenType::STRING); }

.                           { return static_cast<int>(TokenType::ERROR); }

%%
