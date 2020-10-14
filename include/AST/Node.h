#pragma once

#include <memory>
#include "Util/Alias.h"

class ASTVisitor;

class Scope;

/// 节点
class Node : public std::enable_shared_from_this<Node> {
public:
    virtual ~Node() = default;

    virtual void accept(const SharedPtr<ASTVisitor> &visitor) = 0;

    // 当前节点所处的作用域
    SharedPtr<Scope> scope;
};