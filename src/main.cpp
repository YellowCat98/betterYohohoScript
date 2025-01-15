#include <iostream>
#include <yhs/frontend/Lexer.hpp>
#include <yhs/frontend/Parser.hpp>
#include <yhs/Result.hpp>

using namespace yhs::frontend;

int main() {
    try {
        auto parser = yhs::frontend::Parser();

        auto ast = parser.produceAST("2 + (5 + 2 / 2 * 1 % 2)");
        for (auto& node : ast.body) {
            std::visit([](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, AST::BinaryExpr>) {
                    arg.op;
                }
            }, node);
        }
    } catch (const std::runtime_error& e) {
        std::cout << "Err: " << e.what() << std::endl;
    }
    
    return 0;
}