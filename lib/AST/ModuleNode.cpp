#include <utility>
#include "AST/ModuleNode.h"
#include "Sema/ASTVisitor.h"

ModuleNode::ModuleNode(String filename, SharedPtrVector<StmtNode> childStmts) : filename(std::move(filename)), childStmts(std::move(childStmts)) {
}

bool ModuleNode::isEmpty() const {
    return childStmts.empty();
}

void ModuleNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<ModuleNode>(shared_from_this()));
}

void ModuleNode::bindChildrenInversely() {
    auto self = shared_from_this();
    for (const SharedPtr<StmtNode> &stmt: childStmts) {
        stmt->parent = self;
    }
}
