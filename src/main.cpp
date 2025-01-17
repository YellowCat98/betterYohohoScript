#include <iostream>
#include <yhs/frontend/Lexer.hpp>
#include <yhs/frontend/Parser.hpp>
#include <yhs/runtime/interpreter.hpp>
#include <yhs/Result.hpp>

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
    auto i = yhs::runtime::interpreter();
    auto parser = yhs::frontend::Parser();
    auto result = i.evaluate(parser.produceAST("5 + 1"));
    if (result->type == values::Type::Number) {
        try {
            std::cout << "Returned value: " << static_cast<values::NumVal*>(result)->value << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
        
    }
    delete result;
    
    return 0;
}