#pragma once

#include <exception>
#include <utility>
#include "Support/Alias.h"

/// 编译异常
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

/// 语义分析异常
class SemanticException : public CompilerException {
public:
    explicit SemanticException(const String &message) : CompilerException(message) {}

    ~SemanticException() noexcept override = default;
};

/// IR生成异常
class CodeGenException : public CompilerException {
public:
    explicit CodeGenException(const String &message) : CompilerException(message) {}

    ~CodeGenException() noexcept override = default;
};

/// 编译驱动器异常
class DriverException : public CompilerException {
public:
    explicit DriverException(const String &message) : CompilerException(message) {}

    ~DriverException() noexcept override = default;
};