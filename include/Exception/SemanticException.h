#pragma once

#include "Exception/CompilerException.h"

class SemanticException : public CompilerException {
public:
    explicit SemanticException(const String &message) : CompilerException(message) {}

    ~SemanticException() noexcept override = default;
};