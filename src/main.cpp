#include <iostream>
#include <yhs/frontend/Lexer.hpp>
#include <yhs/frontend/Parser.hpp>
#include <yhs/Result.hpp>

using namespace yhs::frontend;
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
    /*
    try {
        auto parser = yhs::frontend::Parser();

        auto program = parser.produceAST("5 + 8 / 2");

        for (auto& node : program->body) {
            if (node->kind == AST::NodeType::BinaryExpr || node->kind == AST::NodeType::NumericLiteral || node->kind == AST::NodeType::Identifier) {
                testExpr(node);
            }
        }

    } catch (const std::runtime_error& e) {
        std::cout << "Err: " << e.what() << std::endl;
    }
    */
    
    return 0;
}