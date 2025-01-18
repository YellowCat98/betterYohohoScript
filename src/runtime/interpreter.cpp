#include <yhs/runtime/interpreter.hpp>
#include <yhs/frontend/AST.hpp>
#include <stdexcept>
#include <iostream>

using namespace yhs::runtime;
using namespace yhs::frontend;

values::RuntimeVal* interpreter::evaluate_program(AST::Program* program, Environment* env) {
    values::RuntimeVal* lastEvaluated = nullptr;

    for (auto& stmt : program->body) {
        lastEvaluated = evaluate(stmt, env);
        std::cout << "a statement hasb een evaluated" << std::endl;
    }
    return lastEvaluated;
}

values::DoubleVal* interpreter::evaluate_numeric_binary_expr(values::RuntimeVal* lhs, values::RuntimeVal* rhs, const std::string& op) {
    double lhsValue = 0;
    double rhsValue = 0;

    if (lhs->type == values::Type::Number) {
        lhsValue = static_cast<values::NumVal*>(lhs)->value;
    } else {
        lhsValue = static_cast<values::DoubleVal*>(lhs)->value;
    }

    if (rhs->type == values::Type::Number) {
        rhsValue = static_cast<values::NumVal*>(rhs)->value;
    } else {
        rhsValue = static_cast<values::DoubleVal*>(rhs)->value;
    }

    double result = 0;
    if (op == "+") result = lhsValue + rhsValue; else
    if (op == "-") result = lhsValue - rhsValue; else
    if (op == "*") result = lhsValue * rhsValue; else
    if (op == "/") result = lhsValue / rhsValue; else
    if (op == "%") result = static_cast<int>(lhsValue) % static_cast<int>(rhsValue);

    std::cout << "lhs: " << lhsValue << std::endl;
    std::cout << "rhs: " << rhsValue << std::endl;
    std::cout << "result: " << result << std::endl;

    return new values::DoubleVal(result);
}

values::RuntimeVal* interpreter::evaluate_binary_expr(AST::BinaryExpr* binEx, Environment* env) {
    auto lhs = evaluate(binEx->left, env);
    auto rhs = evaluate(binEx->right, env);

    delete binEx->left;
    delete binEx->right;
    
    if (
        !( (lhs->type == values::Type::Number && rhs->type == values::Type::Number) ||
            (lhs->type == values::Type::Double && rhs->type == values::Type::Double) ||
            (lhs->type == values::Type::Number && rhs->type == values::Type::Double) ||
            (lhs->type == values::Type::Double && rhs->type == values::Type::Number) )
    ) throw std::runtime_error("lhs or rhs is not of type NumVal or DoubleVal.");

    auto value = evaluate_numeric_binary_expr(lhs, rhs, binEx->op);
    std::cout << "CALCULATED VALUE..." << std::endl;
    return value;
}

values::RuntimeVal* interpreter::evaluate_identifier(AST::Identifier* ident, Environment* env) {
    return env->lookupVar(ident->symbol);
}

values::RuntimeVal* interpreter::evaluate(AST::Stmt* stmt, Environment* env) {
    switch (stmt->kind) {
        case AST::NodeType::NumericLiteral: {
            return new values::NumVal(static_cast<AST::NumericLiteral*>(stmt)->value);
        }
        case AST::NodeType::DoubleLiteral: {
            return new values::DoubleVal(static_cast<AST::DoubleLiteral*>(stmt)->value);
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