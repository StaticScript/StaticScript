#pragma once

#include "Exception/CompilerException.h"

/// IR生成异常
class CodeGenException : public CompilerException {
public:
    explicit CodeGenException(const String &message) : CompilerException(message) {}

    ~CodeGenException() noexcept override = default;
};