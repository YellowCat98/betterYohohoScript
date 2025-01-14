#pragma once
#include <string>

namespace frontend {
    class Lexer {
    public:
        Lexer();

        void tokenize(const std::string& input);
    };
}