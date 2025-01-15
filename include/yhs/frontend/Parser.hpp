#pragma once

#include <yhs/frontend/AST.hpp>
#include <yhs/Result.hpp>
#include <yhs/frontend/Lexer.hpp>

namespace yhs {
    namespace frontend {
        class Parser {
        private:

            std::deque<Lexer::Token> tokens;

            Lexer::Token at();
            Lexer::Token eat();
            Lexer::Token expect();
            bool notEOF();

            AST::Stmt parseStmt();
            AST::Expr parseExpr();
            AST::Expr parseAdditiveExpr();
            AST::Expr parseMultiplicitaveExpr();
            AST::Expr parsePrimaryExpr();
        public:
            AST::Program produceAST(const std::string& code);

            Parser() {}
        };
    }
}