#pragma once

#include <optional>
#include <stdexcept>

namespace yhs {
    template <typename T>
    class Result {
    private:
        std::optional<T> value;
        std::optional<std::string> err;
        bool ok;
    public:
        explicit Result(T value)
            : value(std::move(value)), err(std::nullopt), ok(true) {}
        
        explicit Result(const std::string& err) : value(std::nullopt), err(err), ok(false) {}

        T unwrap() {
            if (isErr()) {
                throw std::runtime_error("Called unwrap on Err value.");
            }
            return *value;
        }
        std::string getErr() {
            return *err;
        }
        bool isOk() const {
            return ok;
        }

        bool isErr() const {
            return !ok;
        }

        static Result<T> Ok(T value) {
            return Result<T>(std::move(value));
        }
        static Result<T> Err(const std::string& err) {
            return Result<T>(err);
        }
    };
}