#pragma once

#include <yhs/frontend/AST.hpp>
#include <yhs/Result.hpp>
#include <yhs/frontend/Lexer.hpp>
#include <iostream>

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
        public:
            AST::Program* produceAST(const std::string& code);

            Parser() {}
        };
    }
}