#include <yhs/runtime/Environment.hpp>
#include <stdexcept>
#include <numbers>

using namespace yhs::runtime;

values::RuntimeVal* Environment::declareVar(const std::string& name, values::RuntimeVal* value, bool constant) {
    if (variables.find(name) != variables.end()) {
        throw std::runtime_error("Variable " + name + " is already declared.");
    }

    variables.insert({name, value});

    if (constant) {
        constants.insert(name);
    }

    return value;
}

values::RuntimeVal* Environment::assignVar(const std::string& name, values::RuntimeVal* value) {
    auto env = this->resolve(name);

    if (env->constants.find(name) != env->constants.end()) {
        throw std::runtime_error("Attempted to reassign constant variable " + name + ".");
    }

    env->variables[name] = value;
    return value;
}

values::RuntimeVal* Environment::lookupVar(const std::string& name) {
    return this->resolve(name)->variables[name];
}

Environment* Environment::resolve(const std::string& name) {
    if (variables.find(name) != variables.end()) {
        return this;
    }

    if (!parent) {
        throw std::runtime_error("Cannot resolve variable " + name + " as it was not found.");
    }

    return parent->resolve(name);
}

Environment* Environment::setupEnv() {
    auto env = new Environment(nullptr);

    env->declareVar("true", new values::BoolVal(true), true);
    env->declareVar("false", new values::BoolVal(false), true);
    env->declareVar("PI", new values::DoubleVal(3.14159265358979323846), true);

    env->declareVar("print", new values::NativeFunVal([](std::vector<values::RuntimeVal*> args, Environment* env) -> values::RuntimeVal* {
        for (auto& arg : args) {
            if (arg->type == values::Type::Double) {
                std::cout << static_cast<values::DoubleVal*>(arg)->value << std::endl;
            } else if (arg->type == values::Type::Number) {
                std::cout << static_cast<values::NumVal*>(arg)->value << std::endl;
            } else {
                delete arg;
                throw std::runtime_error("Cannot call native function print on a value that is not a number or a double.");
            }
        }
        return new values::NullVal();
    }), true);
    return env;
}