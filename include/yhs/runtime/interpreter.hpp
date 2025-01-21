#pragma once

#include <yhs/runtime/values.hpp>
#include <yhs/frontend/AST.hpp>
#include <yhs/runtime/Environment.hpp>

namespace yhs {
    namespace runtime {
        class interpreter {
        private:
            values::RuntimeVal* evaluate_program(frontend::AST::Program* program, Environment* env);

            values::DoubleVal* evaluate_numeric_binary_expr(values::RuntimeVal* lhs, values::RuntimeVal* rhs, const std::string& op);

            values::RuntimeVal* evaluate_binary_expr(frontend::AST::BinaryExpr* binEx, Environment* env);

            values::RuntimeVal* evaluate_identifier(frontend::AST::Identifier* ident, Environment* env);
            
            values::RuntimeVal* evaluate_variable_declaration(frontend::AST::VarDeclaration* varDec, Environment* env);

            values::RuntimeVal* evaluate_assignment_expression(frontend::AST::AssignmentExpr* assignment, Environment* env);

            values::RuntimeVal* evaluate_object_expr(frontend::AST::ObjectLiteral* object, Environment* env);

            values::RuntimeVal* evaluate_member_expr(frontend::AST::MemberExpr* member, Environment* env);

            values::RuntimeVal* evaluate_call_expr(frontend::AST::CallExpr* call, Environment* env);
        public:
            interpreter() {}

            values::RuntimeVal* evaluate(frontend::AST::Stmt* astNode, Environment* env);
        };
    }
}