#include "AST/Module.h"

#include <utility>

Module::Module(std::string filename) : filename(std::move(filename)) {
    empty = true;
}

Module::Module(std::string filename, SharedPtrVector<Stmt> childStmts) : filename(std::move(filename)), childStmts(std::move(childStmts)) {
    empty = false;
}

const std::string &Module::getFilename() const {
    return filename;
}

bool Module::isEmpty() const {
    return empty;
}

void Module::codegen() {

}