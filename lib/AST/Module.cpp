#include "AST/Module.h"

#include <utility>


Module::Module(String filename, SharedPtrVector<Stmt> childStmts) : filename(std::move(filename)), childStmts(std::move(childStmts)) {
}

const String &Module::getFilename() const {
    return filename;
}

bool Module::isEmpty() const {
    return childStmts.empty();
}