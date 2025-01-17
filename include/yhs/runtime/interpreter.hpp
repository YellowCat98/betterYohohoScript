#pragma once

#include <yhs/runtime/values.hpp>
#include <yhs/frontend/AST.hpp>

namespace yhs {
    namespace runtime {
        class interpreter {
        private:
            values::RuntimeVal* evaluate_program(frontend::AST::Program* program);

            values::NumVal* evaluate_numeric_binary_expr(values::NumVal* lhs, values::NumVal* rhs, const std::string& op);

            values::RuntimeVal* evaluate_binary_expr(frontend::AST::BinaryExpr* binEx);
            
        public:
            interpreter() {}

            values::RuntimeVal* evaluate(frontend::AST::Stmt* astNode);
        };
    }
}