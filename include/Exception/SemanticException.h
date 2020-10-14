#pragma once

#include "Exception/CompilerException.h"

/// 语义分析异常
class SemanticException : public CompilerException {
public:
    explicit SemanticException(const String &message) : CompilerException(message) {}

    ~SemanticException() noexcept override = default;
};