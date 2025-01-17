#include <yhs/runtime/interpreter.hpp>
#include <yhs/frontend/AST.hpp>
#include <stdexcept>

using namespace yhs::runtime;
using namespace yhs::frontend;

values::RuntimeVal* interpreter::evaluate_program(AST::Program* program) {
    values::RuntimeVal* lastEvaluated = nullptr;

    for (auto& stmt : program->body) {
        lastEvaluated = evaluate(stmt);
        //delete stmt;
    }
    delete program;
    return lastEvaluated;
}

values::NumVal* interpreter::evaluate_numeric_binary_expr(values::NumVal* lhs, values::NumVal* rhs, const std::string& op) {
    int result = 0;

    if (op == "+") result = lhs->value + rhs->value; else
    if (op == "-") result = lhs->value - rhs->value; else
    if (op == "*") result = lhs->value * rhs->value; else
    if (op == "/") result = lhs->value / rhs->value; else
    if (op == "%") result = lhs->value % rhs->value;

    return new values::NumVal(result);
}

values::RuntimeVal* interpreter::evaluate_binary_expr(AST::BinaryExpr* binEx) {
    auto lhs = evaluate(binEx->left);
    auto rhs = evaluate(binEx->right);

    //delete binEx->left;
    //delete binEx->right;
    
    if (lhs->type == values::Type::Number && rhs->type == values::Type::Number) return evaluate_numeric_binary_expr(static_cast<values::NumVal*>(lhs), static_cast<values::NumVal*>(rhs), binEx->op);

    throw std::runtime_error("lhs or rhs is not an integer.");
}

values::RuntimeVal* interpreter::evaluate(AST::Stmt* stmt) {
    switch (stmt->kind) {
        case AST::NodeType::NumericLiteral: {
            auto numericLiteral = static_cast<AST::NumericLiteral*>(stmt);
            auto num = new values::NumVal(numericLiteral->value);
            return num;
        }
        case AST::NodeType::Program: {
            return evaluate_program(static_cast<AST::Program*>(stmt));
        }
        case AST::NodeType::BinaryExpr: {
            return evaluate_binary_expr(static_cast<AST::BinaryExpr*>(stmt));
        }
        default: {
            return new values::RuntimeVal();
        }
    }
}