#pragma once

#include "Config/Config.h"
#include "Support/LLVM.h"

inline void reportError(const llvm::Twine &reason) {
#ifdef DEBUG_MODE
    llvm::report_fatal_error(reason);
#else
    llvm::errs() << reason << '\n';
    abort();
#endif
}

inline void reportOnError(bool condition, const llvm::Twine &reason) {
    if (condition) {
        reportError(reason);
    }
}

inline void reportTypeError(const llvm::Twine &reason) {
    reportError(llvm::formatv("[Compilation Type Error] {0}", reason));
}

inline void reportOnTypeError(bool condition, const llvm::Twine &reason) {
    if (condition) {
        reportTypeError(reason);
    }
}

inline void reportSemanticError(const llvm::Twine &reason) {
    reportError(llvm::formatv("[Compilation Semantic Error] {0}", reason));
}

inline void reportOnSemanticError(bool condition, const llvm::Twine &reason) {
    if (condition) {
        reportSemanticError(reason);
    }
}

inline void reportLinkError(const llvm::Twine &reason) {
    reportError(llvm::formatv("[Compilation Link Error] {0}", reason));
}

inline void reportOnLinkError(bool condition, const llvm::Twine &reason) {
    if (condition) {
        reportLinkError(reason);
    }
}

inline void reportCodeGenError(const llvm::Twine &reason) {
    reportError(llvm::formatv("[Compilation CodeGen Error] {0}", reason));
}

inline void reportOnCodeGenError(bool condition, const llvm::Twine &reason) {
    if (condition) {
        reportCodeGenError(reason);
    }
}

inline void reportDriverError(const llvm::Twine &reason) {
    reportError(llvm::formatv("[Compilation Driver Error] {0}", reason));
}

inline void reportOnDriverError(bool condition, const llvm::Twine &reason) {
    if (condition) {
        reportDriverError(reason);
    }
}