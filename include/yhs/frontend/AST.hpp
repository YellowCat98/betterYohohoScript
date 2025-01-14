#pragma once

#include <deque>
#include <string>
#include <variant>

#define ALL_AST_NODES Stmt, Expr, BinaryExpr, Identifier, NumericLiteral

namespace yhs {
    namespace frontend {
        class AST {
        public:
            enum class NodeType {
                Program, // 0
                NumericLiteral, // 1
                Identifier, // 2
                BinaryExpr // 3
            };

            struct Stmt {
                Stmt() {};
                virtual ~Stmt() = default;
                NodeType kind;
            };

            

            struct Expr : public Stmt {Expr(){}};

            struct BinaryExpr : public Expr {
                BinaryExpr(Expr left, Expr right, const std::string& op) : left(left), right(right), op(op) {
                    this->kind = NodeType::BinaryExpr;
                }

                Expr left;
                Expr right;
                std::string op;
            };

            struct Identifier : public Expr {
                Identifier(const std::string& symbol) : symbol(symbol) {
                    this->kind = NodeType::Identifier;
                }
                std::string symbol;
            };

            struct NumericLiteral : public Expr {
                NumericLiteral(int value) : value(value) {
                    this->kind = NodeType::NumericLiteral;
                }

                int value;
            };

            struct Program : public Stmt {
                Program(std::deque<std::variant<Stmt, Expr, BinaryExpr, Identifier, NumericLiteral>> body) : body(body) {
                    this->kind = NodeType::Program;
                }
                std::deque<std::variant<Stmt, Expr, BinaryExpr, Identifier, NumericLiteral>> body;
            };
        };
    }
}