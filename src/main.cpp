#include <iostream>
#include <yhs/frontend/Lexer.hpp>
#include <yhs/frontend/Parser.hpp>
#include <yhs/runtime/interpreter.hpp>
#include <yhs/Result.hpp>
#include <yhs/runtime/Environment.hpp>

using namespace yhs::frontend;
using namespace yhs::runtime;
/*
void testExpr(AST::Stmt* expr) {
    if (auto* identifier = dynamic_cast<AST::Identifier*>(expr)) {
        std::cout << "Identifier: " << identifier->symbol << "\n";
    } else if (auto* numericLiteral = dynamic_cast<AST::NumericLiteral*>(expr)) {
        std::cout << "NumericLiteral: " << numericLiteral->value << "\n";
    } else if (auto* binaryExpr = dynamic_cast<AST::BinaryExpr*>(expr)) {
        std::cout << "Binary Expression FOUND." << std::endl;
        //std::cout << "  Left Operand Value: " << typeid(*binaryExpr->left).name() << "\n";
        //std::cout << "  Right Operand Value: " << typeid(*binaryExpr->right).name() << "\n";
        // Recursively test left and right operands
        testExpr(binaryExpr->left);
        std::cout << "BinaryExpr: operator " << binaryExpr->op << "\n";
        testExpr(binaryExpr->right);
    }
    delete expr;
}
*/

int main() {
    try {
    auto i = yhs::runtime::interpreter();
    auto parser = yhs::frontend::Parser();

    auto env = Environment::setupEnv();

    //auto tokens = yhs::frontend::Lexer().tokenize("var x = {x: 5}");
    //for (auto& token : tokens) {
    //    std::cout << "Token type: " << static_cast<int>(token.type) << std::endl;
    //    std::cout << "Token Value: " << token.value << std::endl << std::endl;
    //}

    auto result = i.evaluate(parser.produceAST(R"(
var object = {
    x: 5,
    y: 2,
    z: 2,
    complex: {
        x: 2,
        y: print
    }
};

object[complex][y](object[x] + object[complex][x])

)"), env);
    delete env;
    } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
    }
    
    return 0;
}