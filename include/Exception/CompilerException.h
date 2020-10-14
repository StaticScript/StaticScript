#pragma once

#include <exception>
#include <utility>
#include "Util/Alias.h"

class CompilerException : public std::exception {
public:
    ~CompilerException() noexcept override = default;

    explicit CompilerException(String  message) : message(std::move(message)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }

protected:
    String message;
};