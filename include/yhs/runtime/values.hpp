#pragma once

namespace yhs {
    namespace runtime {
        class values {
            values() = delete; // no constructor

            enum class Type {
                Number // 0
            };

            struct RuntimeVal {
                RuntimeVal() {}
                virtual ~RuntimeVal() = default;

                values::Type type;
            };
            

            struct NumVal : public RuntimeVal {
                NumVal() {
                    type = values::Type::Number;
                }

                int value;
            };
        };
    }
}