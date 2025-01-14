#include <iostream>
#include <yhs/frontend/Lexer.hpp>
#include <yhs/Result.hpp>

int main() {
    auto lexer = yhs::frontend::Lexer();
    auto res = lexer.tokenize(";");

    if (res.isOk()) {
        try {
            auto tokens = res.unwrap();
            for (auto& token : tokens) {
                std::cout << token.value << std::endl;
            }
        } catch (std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }

    } else {
        std::cout << "is not ok.";
    }
    return 0;
}