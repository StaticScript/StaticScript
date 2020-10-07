#ifndef STATICSCRIPT_AST_TYPENODE_H
#define STATICSCRIPT_AST_TYPENODE_H

#include "Util/Alias.h"

enum class TypeKind {
    Unknown, Boolean, Integer, String
};

class TypeNode {
public:
    explicit TypeNode(TypeKind kind);

    virtual ~TypeNode() = default;

    TypeKind kind;
};

// 内建类型
class BuiltinTypeNode : public TypeNode {
public:
    explicit BuiltinTypeNode(TypeKind kind);

    ~BuiltinTypeNode() override = default;

    static inline const SharedPtr<BuiltinTypeNode> BOOLEAN_TYPE = makeShared<BuiltinTypeNode>(TypeKind::Boolean);
    static inline const SharedPtr<BuiltinTypeNode> INTEGER_TYPE = makeShared<BuiltinTypeNode>(TypeKind::Integer);
    static inline const SharedPtr<BuiltinTypeNode> STRING_TYPE = makeShared<BuiltinTypeNode>(TypeKind::String);
    static inline const SharedPtr<BuiltinTypeNode> UNKNOWN_TYPE = makeShared<BuiltinTypeNode>(TypeKind::Unknown);
};

#endif // STATICSCRIPT_AST_TYPENODE_H
