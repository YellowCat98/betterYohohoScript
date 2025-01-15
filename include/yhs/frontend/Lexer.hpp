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
            Int, // 5
            Var, // 6
            Const, // 7
            Semicolon, // 8
            EOF_ // 9
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