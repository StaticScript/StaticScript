#pragma once

#include <memory>
#include "Util/Alias.h"

class ASTVisitor;

class Node : public std::enable_shared_from_this<Node> {
public:
    virtual ~Node() = default;

    virtual void accept(const SharedPtr<ASTVisitor> &visitor) = 0;
};