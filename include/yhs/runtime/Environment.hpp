#pragma once

#include <yhs/runtime/values.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

namespace yhs {
    namespace runtime {
        class Environment {
        private:
            Environment* parent;

            std::unordered_map<std::string, values::RuntimeVal*> variables;
            std::unordered_set<std::string> constants;
        public:
            Environment(Environment* parent) : parent(parent) {
                bool global = this->parent ? true : false;
            }
            ~Environment() {
                for (auto it = this->variables.begin(); it != this->variables.end();) {
                    if (it->second) {
                        delete it->second;
                        it = this->variables.erase(it); // Safely erase the element after deletion
                    } else {
                        ++it;
                    }
                }
                constants.clear();
            }

            values::RuntimeVal* declareVar(const std::string& name, values::RuntimeVal* value, bool constant);
            values::RuntimeVal* assignVar(const std::string& name, values::RuntimeVal* value);
            values::RuntimeVal* lookupVar(const std::string& name);
            Environment* resolve(const std::string& name);

            static Environment* setupEnv();
        };
    }
}