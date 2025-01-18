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
        case Lexer::TokenType::Number: {
            return new AST::NumericLiteral(std::stoi(eat().value));
        }
        case Lexer::TokenType::Double: {
            return new AST::DoubleLiteral(std::stod(eat().value));
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

AST::Stmt* Parser::parseVarDeclaration() {
    bool constant = eat().type == Lexer::TokenType::Const;
    std::string identifier = expect(Lexer::TokenType::Identifier, "Expected Identifier following variable declaration keyword.").value;

    if (at().type == Lexer::TokenType::Semicolon) {
        eat();
        if (constant) {
            throw std::runtime_error("Must assign value to constant variable.");
        }

        return new AST::VarDeclaration(false, identifier, {});
    }

    expect(Lexer::TokenType::Equals, "Expected equals sign after identifier in variable declaration.");

    auto declaration = new AST::VarDeclaration(constant, identifier, parseExpr());
}

AST::Stmt* Parser::parseStmt() {
    switch (at().type) {
        case Lexer::TokenType::Var: {
            return parseVarDeclaration();
        }
        case Lexer::TokenType::Const: {
            return parseVarDeclaration();
        }
        default: {
            return parseExpr();
        }
    }
}

AST::Program* Parser::produceAST(const std::string& code) {
    tokens = Lexer().tokenize(code);

    auto program = new AST::Program({});

    while (notEOF()) {
        program->body.push_back(parseStmt());
    }

    return program;
}