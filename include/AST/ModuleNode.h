#pragma once

#include "AST/Node.h"
#include "AST/StmtNode.h"

/// 模块节点
class ModuleNode final : public Node {
public:
    ~ModuleNode() override = default;

    explicit ModuleNode(String filename, SharedPtrVector<StmtNode> childStmts);

    virtual bool isEmpty() const;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    String filename;
    SharedPtrVector<StmtNode> childStmts;

    SharedPtr<Scope> internalScope;
};
