#include <yhs/frontend/Parser.hpp>

using namespace yhs::frontend;
using namespace yhs;

Lexer::Token Parser::at() {
    return tokens[0];
}

Lexer::Token Parser::eat() {
    auto prev = tokens.front();
    tokens.pop_front();
    return prev;
}

Lexer::Token Parser::expect(Lexer::TokenType type, const std::string& err) {
    auto prev = tokens.front();
    tokens.pop_front();
    if (prev.type != type) {
        throw std::runtime_error("Error: " + err);
    }
    return prev;
}

bool Parser::notEOF() {
    return tokens[0].type != Lexer::TokenType::EOF_;
}

AST::Expr* Parser::parseAdditiveExpr() {
    auto left = parseMultiplicitaveExpr();

    while (at().value == "+" || at().value == "-") {
        auto op = eat().value;
        auto right = parseMultiplicitaveExpr();
        left = new AST::BinaryExpr(left, right, op);
    }

    return left;
}

AST::Expr* Parser::parseMultiplicitaveExpr() {
    auto left = parsePrimaryExpr();

    while (at().value == "/" || at().value == "*" || at().value == "%") {
        auto op = eat().value;
        auto right = parsePrimaryExpr();
        left = new AST::BinaryExpr(left, right, op);
    }

    return left;
}

AST::Expr* Parser::parsePrimaryExpr() {
    auto tk = at().type;

    switch (tk) {
        case Lexer::TokenType::Identifier: {
            return new AST::Identifier(eat().value);
        }
        case Lexer::TokenType::Int: {
            return new AST::NumericLiteral(std::stoi(eat().value));
        }
        case Lexer::TokenType::OpenParen: {
            eat();
            auto value = parseExpr();
            expect(Lexer::TokenType::CloseParen, "Expected closing parenthesis.");
            return value;
        }
        default: {
            throw std::runtime_error("Unexpected token found during parsing.");
        }
    }
}

AST::Expr* Parser::parseExpr() {
    return parseAdditiveExpr();
}

AST::Stmt* Parser::parseStmt() {
    return parseExpr();
}

AST::Program* Parser::produceAST(const std::string& code) {
    tokens = Lexer().tokenize(code);

    auto program = new AST::Program({});

    while (notEOF()) {
        program->body.push_back(parseStmt());
    }

    return program;
}