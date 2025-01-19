#pragma once

#include <deque>
#include <string>
#include <variant>
#include <optional>
#include <vector>


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
                VarDeclaration, // 5
                AssignmentExpr, // 6
                CallExpr, // 7
                ObjectLiteral, // 8
                Property // 9
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

            struct AssignmentExpr: public Expr {
                AssignmentExpr(Expr* assigne, Expr* value) : assigne(assigne), value(value) {
                    this->kind = NodeType::AssignmentExpr;
                }

                Expr* assigne;
                Expr* value;
            };

            struct Property : public Expr {
                Property(const std::string& key, Expr* value) : key(key), value(value) {
                    this->kind = NodeType::Property;
                }

                std::string key;
                Expr* value;
            };

            struct ObjectLiteral : public Expr {
                ObjectLiteral(std::vector<Property*> properties) : properties(properties) {
                    this->kind = NodeType::ObjectLiteral;
                }

                std::vector<Property*> properties;
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