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
    auto left = parseMemberCallExpr();

    while (at().value == "/" || at().value == "*" || at().value == "%") {
        auto op = eat().value;
        auto right = parseMemberCallExpr();
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
            throw std::runtime_error("Unexpected token found during parsing: '" + at().value + "'");
        }
    }
}

AST::Expr* Parser::parseExpr() {
    return parseAssignmentExpr();
}

AST::Expr* Parser::parseMemberCallExpr() {
    auto member = parseMemberExpr();

    if (at().type == Lexer::TokenType::OpenParen) {
        return parseCallExpr(member);
    }

    return member;
}

AST::Expr* Parser::parseAssignmentExpr() {
    auto left = parseObjectExpr();

    if (at().type == Lexer::TokenType::Equals) {
        eat();
        auto value = parseAssignmentExpr();
        return new AST::AssignmentExpr(left, value);
    }

    return left;
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

    expect(Lexer::TokenType::Semicolon, "Expceted semicolon after variable declaration.");

    return declaration;
}

AST::Expr* Parser::parseObjectExpr() {
    if (at().type != Lexer::TokenType::OpenBrace) {
        return parseAdditiveExpr();
    }

    eat();
    std::vector<AST::Property*> properties = {};

    while (notEOF() && at().type != Lexer::TokenType::CloseBrace) {
        auto key = expect(Lexer::TokenType::Identifier, "Object literal key expected.").value;

        expect(Lexer::TokenType::Colon, "Expected colon following identifier in object expression.");

        auto value = parseExpr();

        properties.push_back(new AST::Property(key, value));
        if (at().type != Lexer::TokenType::CloseBrace) {
            expect(Lexer::TokenType::Comma, "Expected comma or closing brace following property.");
        }
    }

    expect(Lexer::TokenType::CloseBrace, "Object literal missing closing brace.");
    return new AST::ObjectLiteral(properties);
}

AST::Expr* Parser::parseCallExpr(AST::Expr* caller) {
    AST::Expr* callExpr = new AST::CallExpr(parseArgs(), caller);

    if (at().type == Lexer::TokenType::OpenParen) {
        callExpr = parseCallExpr(callExpr);
    }

    return callExpr;
}

AST::Expr* Parser::parseMemberExpr() {
    AST::Expr* object = parsePrimaryExpr();

    while (at().type == Lexer::TokenType::OpenBrack) {
        eat();
        auto property = parseExpr();
        if (property->kind != AST::NodeType::Identifier) {
            delete property;
            delete object;
            throw std::runtime_error("Expected property to be an identifier.");
        }
        expect(Lexer::TokenType::CloseBrack, "Expected closing bracket.");

        object = new AST::MemberExpr(object, property);
    }

    return object;
}

std::vector<AST::Expr*> Parser::parseArgs() {
    expect(Lexer::TokenType::OpenParen, "Expected opening parenthesis.");

    auto args = parseArgumentsList();

    expect(Lexer::TokenType::CloseParen, "Expected closing parenthesis in arguments list.");
    return args;
}

std::vector<AST::Expr*> Parser::parseArgumentsList() {
    std::vector<AST::Expr*> args = {};
    while (notEOF() && at().type != Lexer::TokenType::CloseParen) {
        args.push_back(parseExpr());

        if (at().type == Lexer::TokenType::Comma) {
            eat();
        } else if (at().type != Lexer::TokenType::CloseParen) {
            throw std::runtime_error("Expected comma or closing parenthesis.");
        }
    }

    return args;
}

AST::Stmt* Parser::parseFunDeclaration() {
    eat();

    auto name = expect(Lexer::TokenType::Identifier, "Expected identifier after function declaration.").value;

    auto args = parseArgs();

    std::vector<std::string> params = {};

    for (auto& arg : args) {
        if (arg->kind != AST::NodeType::Identifier) {
            delete arg;
            throw std::runtime_error("Expected identifier in function declaration arguments.");
        }

        params.push_back(static_cast<AST::Identifier*>(arg)->symbol);
    }

    expect(Lexer::TokenType::OpenBrace, "Expected open brace after function declaration");

    std::vector<AST::Stmt*> body = {};

    while (notEOF() && at().type != Lexer::TokenType::CloseBrace) {
        body.push_back(parseStmt());
    }

    return new AST::FunDeclaration(params, name, body);
}

AST::Stmt* Parser::parseStmt() {
    switch (at().type) {
        case Lexer::TokenType::Var: {
            return parseVarDeclaration();
        }
        case Lexer::TokenType::Const: {
            return parseVarDeclaration();
        }
        case Lexer::TokenType::Fun: {
            return parseFunDeclaration();
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