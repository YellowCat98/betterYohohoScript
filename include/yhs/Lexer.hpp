#pragma once
#include <string>
#include <deque>
#include <functional>

namespace yhs {
    namespace frontend {
        class Lexer {
        enum class TokenType {
            OpenParen, // 0
            CloseParen, // 1
            BinaryOperator, // 2
            Equals, // 3
            Identifier, // 4
            Number // 5
        };

        struct Token {
            Token(const std::string& value, TokenType type) : value(value), type(type) {}
            TokenType type;
            std::string value;
        };
        

        public:
            Lexer() {}

            void tokenize(const std::string& source);
        };
    }
}