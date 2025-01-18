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
                DoubleLiteral, // 2
                Identifier, // 3
                BinaryExpr, // 4
                VarDeclaration // 5
            };

            struct Stmt {
                Stmt() {};
                virtual ~Stmt() = default;
                NodeType kind;
            };

            

            struct Expr : public Stmt {Expr(){}};

            struct BinaryExpr : public Expr {
                BinaryExpr(Expr* left, Expr* right, const std::string& op) : left(left), right(right), op(op) {
                    this->kind = NodeType::BinaryExpr;
                }
                ~BinaryExpr() {
                    delete left;
                    delete right;
                }

                Expr* left;
                Expr* right;
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

            struct DoubleLiteral : public Expr {
                DoubleLiteral(double value) : value(value) {
                    this->kind = NodeType::DoubleLiteral;
                }

                double value;
            };

            struct VarDeclaration : public Stmt {
                VarDeclaration(bool constant, std::string identifier, std::optional<Expr*> value) : constant(constant), identifier(identifier), value(value) {
                    this->kind = NodeType::VarDeclaration;
                }

                bool constant;
                std::string identifier;
                std::optional<Expr*> value;
            };

            struct Program : public Stmt {
                Program(std::deque<Stmt*> body) : body(body) {
                    this->kind = NodeType::Program;
                }
                ~Program() {
                    for (auto& node : body) {
                        delete node;
                    }
                }
                std::deque<Stmt*> body;
            };
        };
    }
}