#include <yhs/runtime/interpreter.hpp>
#include <yhs/frontend/AST.hpp>
#include <stdexcept>

using namespace yhs::runtime;
using namespace yhs::frontend;

values::RuntimeVal* interpreter::evaluate_program(AST::Program* program, Environment* env) {
    values::RuntimeVal* lastEvaluated = nullptr;

    for (auto& stmt : program->body) {
        lastEvaluated = evaluate(stmt, env);
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

values::RuntimeVal* interpreter::evaluate_binary_expr(AST::BinaryExpr* binEx, Environment* env) {
    auto lhs = evaluate(binEx->left, env);
    auto rhs = evaluate(binEx->right, env);

    //delete binEx->left;
    //delete binEx->right;
    
    if (lhs->type == values::Type::Number && rhs->type == values::Type::Number) return evaluate_numeric_binary_expr(static_cast<values::NumVal*>(lhs), static_cast<values::NumVal*>(rhs), binEx->op);

    throw std::runtime_error("lhs or rhs is not an integer.");
}

values::RuntimeVal* interpreter::evaluate_identifier(AST::Identifier* ident, Environment* env) {
    return env->lookupVar(ident->symbol);
}

values::RuntimeVal* interpreter::evaluate(AST::Stmt* stmt, Environment* env) {
    switch (stmt->kind) {
        case AST::NodeType::NumericLiteral: {
            auto numericLiteral = static_cast<AST::NumericLiteral*>(stmt);
            auto num = new values::NumVal(numericLiteral->value);
            return num;
        }
        case AST::NodeType::Program: {
            return evaluate_program(static_cast<AST::Program*>(stmt), env);
        }
        case AST::NodeType::BinaryExpr: {
            return evaluate_binary_expr(static_cast<AST::BinaryExpr*>(stmt), env);
        }
        case AST::NodeType::Identifier: {
            return evaluate_identifier(static_cast<AST::Identifier*>(stmt), env);
        }
        default: {
            return new values::RuntimeVal();
        }
    }
}