#pragma once

//#include <yhs/runtime/Environment.hpp>

namespace yhs {
    namespace runtime {
        class Environment;
        class values {
            values() = delete; // no constructor
        public:
            enum class Type {
                Number // 0
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
        };
    }
}