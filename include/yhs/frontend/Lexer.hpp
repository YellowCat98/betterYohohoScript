#pragma once
#include <string>
#include <deque>
#include <functional>
#include <yhs/Result.hpp>

namespace yhs {
    namespace frontend {
        class Lexer {
        public:
        enum class TokenType {
            OpenParen, // 0
            CloseParen, // 1
            BinaryOperator, // 2
            Equals, // 3
            Identifier, // 4
            Number, // 5
            Double, // 6
            Var, // 7
            Const, // 8
            Semicolon, // 9
            OpenBrace, // 10
            CloseBrace, // 11
            Colon, // 12
            Comma, // 13
            EOF_ // 14
        };


        struct Token {
            Token(const std::string& value, TokenType type) : value(value), type(type) {}
            TokenType type;
            std::string value;
        };
        

            Lexer() {}

            std::deque<Token> tokenize(const std::string& source);

        
        std::unordered_map<std::string, Lexer::TokenType> reserved {
            {"var", Lexer::TokenType::Var},
            {"const", Lexer::TokenType::Const}
        };

        };
    }
}