#pragma once

#include "Exception/CompilerException.h"

/// 编译驱动器异常
class DriverException : public CompilerException {
public:
    explicit DriverException(const String &message) : CompilerException(message) {}

    ~DriverException() noexcept override = default;
};