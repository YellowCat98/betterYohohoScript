#pragma once

#include <variant>

namespace yhs {
    namespace runtime {
        class Environment;
        class values {
            values() = delete; // no constructor
        public:
            enum class Type {
                Number, // 0
                Double, // 1
                Bool, // 2
                Null // 3
            };

            struct RuntimeVal {
                RuntimeVal() {}
                virtual ~RuntimeVal() = default;

                values::Type type;
            };
            

            struct NumVal : public RuntimeVal {
                NumVal(int value) : value(value) {
                    type = values::Type::Number;
                }

                int value;
            };

            struct DoubleVal : public RuntimeVal {
                DoubleVal(double value) : value(value) {
                    type = values::Type::Double;
                }

                double value;
            };

            struct BoolVal : public RuntimeVal {
                BoolVal(bool value) : value(value) {
                    type = values::Type::Bool;
                }

                bool value;
            };

            struct NullVal : public RuntimeVal {
                NullVal() {
                    type = values::Type::Null;
                }
                std::nullptr_t value = nullptr;
            };
        };
    }
}