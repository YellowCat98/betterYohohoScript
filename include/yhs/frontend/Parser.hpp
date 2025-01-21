#pragma once

#include <yhs/frontend/AST.hpp>
#include <yhs/Result.hpp>
#include <yhs/frontend/Lexer.hpp>
#include <iostream>
#include <vector>

namespace yhs {
    namespace frontend {
        class Parser {
        private:

            std::deque<Lexer::Token> tokens;

            Lexer::Token at();
            Lexer::Token eat();
            Lexer::Token expect(Lexer::TokenType type, const std::string& err);
            bool notEOF();

            AST::Stmt* parseStmt();
            AST::Expr* parseExpr();
            AST::Expr* parseAdditiveExpr();
            AST::Expr* parseMultiplicitaveExpr();
            AST::Expr* parsePrimaryExpr();
            AST::Stmt* parseVarDeclaration();
            AST::Expr* parseAssignmentExpr();
            AST::Expr* parseObjectExpr();
            AST::Expr* parseMemberExpr();
            AST::Expr* parseMemberCallExpr();
            AST::Expr* parseCallExpr(AST::Expr* caller);
            std::vector<AST::Expr*> parseArgumentsList();
            std::vector<AST::Expr*> parseArgs();
            AST::Stmt* parseFunDeclaration();
        public:
            AST::Program* produceAST(const std::string& code);

            Parser() {}
        };
    }
}