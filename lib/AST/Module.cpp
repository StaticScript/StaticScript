#include "AST/Module.h"

#include <utility>

Module::Module(String filename) : filename(std::move(filename)) {
    empty = true;
}

Module::Module(String filename, SharedPtrVector<Stmt> childStmts) : filename(std::move(filename)), childStmts(std::move(childStmts)) {
    empty = false;
}

const String &Module::getFilename() const {
    return filename;
}

bool Module::isEmpty() const {
    return empty;
}

void Module::codegen() {

}