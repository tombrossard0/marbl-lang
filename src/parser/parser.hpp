#pragma once

#include <cstdarg>
#include <lexer.hpp>
#include <memory>

#include "ast.hpp"
#include "parser_exception.hpp"
#include "tokens.hpp"

class Parser {
  public:
    Lexer lexer;

    Parser(std::istream &input, std::string input_name) : lexer(input, input_name) {}

    std::vector<UniqueStmt> parse() {
        std::vector<UniqueStmt> statements;
        while (!isAtEnd()) { statements.push_back(declaration()); }

        return statements;
    }

    bool isAtEnd() { return peek().type == TokenType::T_EOF; }

  private:
    Token previousToken;

    Token peek() {
        if (lexer.currentToken.type == TokenType::T_SOF) lexer.nextToken();
        return lexer.currentToken;
    }

    Token advance() {
        // Consumes the current token and returns it
        previousToken = lexer.currentToken;
        if (!isAtEnd()) lexer.nextToken();
        return previousToken;
    }

    bool check(TokenType type) {
        if (isAtEnd()) return false;
        return peek().type == type;
    }

    template <typename... Types> bool match(Types... types) {
        for (TokenType t : {types...}) {
            if (check(t)) {
                advance();
                return true;
            }
        }

        return false;
    }

    void synchronize() {
        advance();

        while (!isAtEnd()) {
            if (previousToken.type == SEMICOLON) return;

            switch (peek().type) {
            case CLASS | FUN | LET | FOR | IF | WHILE | PRINT | RETURN:
                return;
            default:
                break;
            }

            advance();
        }
    }

    Token consume(TokenType type, std::string msg) {
        if (check(type)) return advance();
        throw ParserException(peek(), msg);
    }

    UniqueExpr primary() {
        // primary        ::= "true" | "false" | "this"
        //                |   NUMBER | STRING | IDENTIFIER | "(" expression ")"
        //                |   "super" "." IDENTIFIER ; // TODO

        if (match(TRUE)) return std::make_unique<Literal>(true);
        if (match(FALSE)) return std::make_unique<Literal>(false);
        // if (match(THIS)) return std::make_unique<Literal>(THIS); // TODO

        if (match(NUMBER, STRING)) return std::make_unique<Literal>(previousToken.literal);
        if (match(IDENTIFIER)) return std::make_unique<Variable>(previousToken);

        if (match(LEFT_PAREN)) {
            UniqueExpr expr = expression();
            consume(RIGHT_PAREN, "Except ')' after expression.");
            return std::make_unique<Grouping>(std::move(expr));
        }

        throw ParserException(peek(), "Expect expression.");
    }

    UniqueExpr call() {
        // call           ::= primary ( "(" arguments? ")" | "." IDENTIFIER )* ;
        UniqueExpr expr = primary();

        return expr;
    }

    UniqueExpr unary() {
        // unary          ::= ( "!" | "-" ) unary | call ;
        if (match(BANG, MINUS)) {
            Token op = previousToken;
            UniqueExpr right = unary();
            return std::make_unique<Unary>(op, std::move(right));
        }

        return call();
    }

    UniqueExpr factor() {
        // factor         ::= unary ( ( "/" | "*" ) unary )* ;
        UniqueExpr expr = unary();

        while (match(SLASH, STAR)) {
            Token op = previousToken;

            UniqueExpr right = unary();
            expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    UniqueExpr term() {
        // term           ::= factor ( ( "-" | "+" ) factor )* ;
        UniqueExpr expr = factor();

        while (match(MINUS, PLUS)) {
            Token op = previousToken;

            UniqueExpr right = factor();
            expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    UniqueExpr comparison() {
        // comparison     ::= term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
        UniqueExpr expr = term();

        while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
            Token op = previousToken;

            UniqueExpr right = term();
            expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    UniqueExpr equality() {
        // equality       ::= comparison ( ( "!=" | "==" ) comparison )* ;
        UniqueExpr expr = comparison();

        while (match(BANG_EQUAL, EQUAL_EQUAL)) {
            Token op = previousToken;

            UniqueExpr right = comparison();
            expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    UniqueExpr logic_and() {
        // logic_and      → equality ( "and" equality )* ;
        UniqueExpr res = equality();

        while (match(AND)) {
            Token op = previousToken;
            UniqueExpr right = equality();
            res = std::make_unique<Logical>(std::move(res), op, std::move(right));
        }

        return res;
    }

    UniqueExpr logic_or() {
        // logic_or       → logic_and ( "or" logic_and )* ;
        UniqueExpr res = logic_and();

        while (match(OR)) {
            Token op = previousToken;
            UniqueExpr right = logic_and();
            res = std::make_unique<Logical>(std::move(res), op, std::move(right));
        }

        return res;
    }

    UniqueExpr assignment() {
        // assignment     ::= ( call "." )? IDENTIFIER "=" assignment
        //                |   logic_or ;
        UniqueExpr expr = logic_or();

        if (match(EQUAL)) {
            Token equals = previousToken;
            UniqueExpr value = assignment();

            if (auto *var = dynamic_cast<Variable *>(expr.get())) {
                Token name = var->name;
                return std::make_unique<Assign>(name, std::move(value));
            }

            throw std::runtime_error("Invalid assignment target.");
        }

        return expr;
    }

    UniqueExpr expression() {
        // expression     ::= assignment ;
        return assignment();
    }

    UniqueStmt expressionStatement() {
        // exprStmt        ::= expression ";" ;
        UniqueExpr value = expression();
        consume(SEMICOLON, "Expect ';' after value.");
        return std::make_unique<Expression>(std::move(value));
    }

    UniqueStmt printStatement() {
        // printStmt       ::= "print" expression ";" ;
        UniqueExpr value = expression();
        consume(SEMICOLON, "Expect ';' after value.");
        return std::make_unique<Print>(std::move(value));
    }

    std::vector<UniqueStmt> block() {
        // block           ::= "{" declaration* "}" ;
        std::vector<UniqueStmt> statements{};

        while (!check(RIGHT_BRACE) && !isAtEnd()) statements.push_back(declaration());

        consume(RIGHT_BRACE, "Expect '}' at end of block.");
        return std::move(statements);
    }

    UniqueStmt ifStatement() {
        consume(LEFT_PAREN, "Expect '(' after 'if'.");
        UniqueExpr condition = expression();
        consume(RIGHT_PAREN, "Expect ')' after if condition.");

        UniqueStmt thenBranch = statement();
        UniqueStmt elseBranch = nullptr;
        if (match(ELSE)) elseBranch = statement();

        return std::make_unique<If>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
    }

    UniqueStmt whileStatement() {
        consume(LEFT_PAREN, "Expect '(' after 'while'.");
        UniqueExpr condition = expression();
        consume(RIGHT_PAREN, "Expect ')' after while condition.");

        UniqueStmt body = statement();
        return std::make_unique<While>(std::move(condition), std::move(body));
    }

    UniqueStmt statement() {
        // statement       ::= exprStmt
        //                 |   forStmt
        //                 |   ifStmt
        //                 |   printStmt
        //                 |   returnStmt
        //                 |   whileStmt
        //                 |   block ;
        if (match(PRINT)) return printStatement();
        if (match(WHILE)) return whileStatement();
        if (match(LEFT_BRACE)) return std::make_unique<Block>(block());
        if (match(IF)) return ifStatement();

        return expressionStatement();
    }

    UniqueStmt letDeclaration() {
        // letDecl         ::= "let" IDENTIFIER ":" IDENTIFIER ( "=" expression )? ";" ;
        Token name = consume(IDENTIFIER, "Expect variable name.");

        UniqueExpr initializer = nullptr;
        if (match(EQUAL)) { initializer = expression(); }

        consume(SEMICOLON, "Expect ';' after variable declaration");
        return std::make_unique<Let>(name, std::move(initializer));
    }

    UniqueStmt declaration() {
        // declaration     ::= classDecl
        //                 |   funDecl
        //                 |   letDecl
        //                 |   statement ;
        try {
            if (match(LET)) return letDeclaration();
            return statement();
        } catch (ParserException err) {
            synchronize();
            return nullptr;
        }
    }
};
