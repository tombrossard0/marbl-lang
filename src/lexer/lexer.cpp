#include "lexer.hpp"

#include <sstream>

#include <FlexLexer.h>

std::vector<Token> Lexer::scanTokens() {
    std::vector<Token> tokens;

    std::istringstream input(source);
    yyFlexLexer scanner(&input);

    int tok;
    while ((tok = scanner.yylex()) != 0) {
        // Build Token object per token
        tokens.emplace_back(static_cast<TokenType>(tok), scanner.YYText(),
                            "literal", // or other info
                            scanner.lineno());
    }

    return tokens;
}
