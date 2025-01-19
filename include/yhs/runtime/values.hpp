#pragma once

#include <unordered_map>
#include <string>

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
                Object, // 3
                Null // 4
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

            struct ObjectVal : public RuntimeVal {
                ObjectVal(std::unordered_map<std::string, RuntimeVal*> properties)  : properties(properties) {
                    type = values::Type::Object;
                }

                std::unordered_map<std::string, RuntimeVal*> properties;
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