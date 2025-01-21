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
            Fun, // 9
            Semicolon, // 10
            OpenBrace, // 11
            CloseBrace, // 12
            OpenBrack, // 13
            CloseBrack, // 14
            Colon, // 15
            Comma, // 16
            EOF_ // 17
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
            {"const", Lexer::TokenType::Const},
            {"fun", Lexer::TokenType::Fun}
        };

        };
    }
}