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

    return new values::DoubleVal(result);
}

values::RuntimeVal* interpreter::evaluate_binary_expr(AST::BinaryExpr* binEx, Environment* env) {
    auto lhs = evaluate(binEx->left, env);
    auto rhs = evaluate(binEx->right, env);

    
    if (
        !( (lhs->type == values::Type::Number && rhs->type == values::Type::Number) ||
            (lhs->type == values::Type::Double && rhs->type == values::Type::Double) ||
            (lhs->type == values::Type::Number && rhs->type == values::Type::Double) ||
            (lhs->type == values::Type::Double && rhs->type == values::Type::Number) )
    ) throw std::runtime_error("lhs or rhs is not of type NumVal or DoubleVal.");

    auto value = evaluate_numeric_binary_expr(lhs, rhs, binEx->op);
    return value;
}

values::RuntimeVal* interpreter::evaluate_identifier(AST::Identifier* ident, Environment* env) {
    return env->lookupVar(ident->symbol);
}

values::RuntimeVal* interpreter::evaluate_variable_declaration(AST::VarDeclaration* varDec, Environment* env) {
    auto value = varDec->value.has_value() ? evaluate(varDec->value.value(), env) : new values::NullVal();

    return env->declareVar(varDec->identifier, value, varDec->constant);
}

values::RuntimeVal* interpreter::evaluate_assignment_expression(AST::AssignmentExpr* assignment, Environment* env) {
    if (assignment->assigne->kind != AST::NodeType::Identifier) {
        throw std::runtime_error("Expected an identifier for assigne in assignment expression.");
    }

    auto name = static_cast<AST::Identifier*>(assignment->assigne)->symbol;
    return env->assignVar(name, evaluate(assignment->value, env));
}

values::RuntimeVal* interpreter::evaluate_object_expr(AST::ObjectLiteral* object, Environment* env) {
    std::unordered_map<std::string, values::RuntimeVal*> properties;
    for (auto& property : object->properties) {
        if (properties.find(property->key) != properties.end()) {
            throw std::runtime_error("Property with the same key already exists: " + property->key);
        }

        properties.insert({property->key, evaluate(property->value, env)});
    }

    return new values::ObjectVal(properties);
}

values::RuntimeVal* interpreter::evaluate_member_expr(AST::MemberExpr* memberExpr, Environment* env) {
    auto object_ = evaluate(memberExpr->object, env);
    
    auto name = static_cast<AST::Identifier*>(memberExpr->property)->symbol;

    if (object_->type == values::Type::Object) {
        auto object = static_cast<values::ObjectVal*>(object_);

        auto it = object->properties.find(name);
        if (it != object->properties.end()) {
            return it->second;
        } else {
            throw std::runtime_error("Property " + it->first + " Does not exist in object.");
        }
    }

    throw std::runtime_error("Attempted to access a property on a non-object type.");
}

values::RuntimeVal* interpreter::evaluate_call_expr(AST::CallExpr* call, Environment* env) {
    std::vector<values::RuntimeVal*> args = {};
    for (auto& arg : call->args) {
        args.push_back(evaluate(arg, env));
    }

    auto fn = evaluate(call->caller, env);
    if (fn->type == values::Type::NativeFun) return static_cast<values::NativeFunVal*>(fn)->call(args, env);

    if (fn->type == values::Type::Fun) {
        auto func = static_cast<values::FunValue*>(fn);
        auto scope = new Environment(func->decEnv);

        for (int i = 0; i < func->params.size(); ++i) {
            scope->declareVar(func->params[i], args[i], false);
        }

        values::RuntimeVal* result = nullptr;

        for (auto& stmt : func->body) {
            result = evaluate(stmt, scope);
        }

        return result;
    }

    delete fn;
    throw std::runtime_error("Cannot call value that is not a function.");
}

values::RuntimeVal* interpreter::evaluate_fun_declaration(AST::FunDeclaration* fun, Environment* env) {
    return env->declareVar(fun->name, new values::FunValue(fun->name, fun->parameters, env, fun->body), true);
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
        case AST::NodeType::VarDeclaration: {
            return evaluate_variable_declaration(static_cast<AST::VarDeclaration*>(stmt), env);
        }
        case AST::NodeType::AssignmentExpr: {
            return evaluate_assignment_expression(static_cast<AST::AssignmentExpr*>(stmt), env);
        }
        case AST::NodeType::ObjectLiteral: {
            return evaluate_object_expr(static_cast<AST::ObjectLiteral*>(stmt), env);
        }
        case AST::NodeType::MemberExpr: {
            return evaluate_member_expr(static_cast<AST::MemberExpr*>(stmt), env);
        }
        case AST::NodeType::CallExpr: {
            return evaluate_call_expr(static_cast<AST::CallExpr*>(stmt), env);
        }
        case AST::NodeType::FunDeclaration: {
            return evaluate_fun_declaration(static_cast<AST::FunDeclaration*>(stmt), env);
        }
        default: {
            delete stmt;
            throw std::runtime_error("This interpreter was not yet setup to interpret this program."); // you'd rarely encounter this error because all ast nodes are evalauted here! (probably)
        }
    }
}