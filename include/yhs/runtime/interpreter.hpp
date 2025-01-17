#pragma once

#include <yhs/runtime/values.hpp>
#include <yhs/frontend/AST.hpp>
#include <yhs/runtime/Environment.hpp>

namespace yhs {
    namespace runtime {
        class interpreter {
        private:
            values::RuntimeVal* evaluate_program(frontend::AST::Program* program, Environment* env);

            values::NumVal* evaluate_numeric_binary_expr(values::NumVal* lhs, values::NumVal* rhs, const std::string& op);

            values::RuntimeVal* evaluate_binary_expr(frontend::AST::BinaryExpr* binEx, Environment* env);

            values::RuntimeVal* evaluate_identifier(frontend::AST::Identifier* ident, Environment* env);
            
        public:
            interpreter() {}

            values::RuntimeVal* evaluate(frontend::AST::Stmt* astNode, Environment* env);
        };
    }
}