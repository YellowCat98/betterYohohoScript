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
    try {
    auto i = yhs::runtime::interpreter();
    auto parser = yhs::frontend::Parser();
    auto result = i.evaluate(parser.produceAST("(25 * 4)+(16 / 2)-(7 * 3) + (100 / 5)"));
    if (result->type == values::Type::Number) {
        std::cout << "Returned value: " << static_cast<values::NumVal*>(result)->value << std::endl;
        } 
        delete result;
    } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
    return 0;
}