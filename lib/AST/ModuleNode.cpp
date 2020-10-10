#include "AST/ModuleNode.h"
#include <utility>

ModuleNode::ModuleNode(String filename, SharedPtrVector<StmtNode> childStmts) : filename(std::move(filename)), childStmts(std::move(childStmts)) {
}

const String &ModuleNode::getFilename() const {
    return filename;
}

bool ModuleNode::isEmpty() const {
    return childStmts.empty();
}