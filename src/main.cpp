#include <iostream>
#include <yhs/frontend/Lexer.hpp>
#include <yhs/Result.hpp>

int main() {
    auto lexer = yhs::frontend::Lexer();
    try {
        auto tokens = lexer.tokenize("var myVariable = 42 + (5 + 2 / 2 * 1 % 2); // variable declaration");
        for (auto& token : tokens) {
            std::cout << "Token: " << token.value << std::endl << "Type: " << static_cast<int>(token.type) << std::endl << std::endl;
        }
    } catch (const std::runtime_error& e) {
        std::cout << "Err: " << e.what() << std::endl;
    }
    
    return 0;
}