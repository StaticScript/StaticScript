#ifndef STATICSCRIPT_AST_TYPE_H
#define STATICSCRIPT_AST_TYPE_H

#include "AST/Base.h"

enum class TypeKind {
    Unknown, Boolean, Integer, String
};

class Type : public ASTNode {
public:
    explicit Type(TypeKind kind);

    ~Type() override = default;

    TypeKind kind;
};

// 内建类型
class BuiltinType : public Type {
public:
    explicit BuiltinType(TypeKind kind);

    ~BuiltinType() override = default;

    static inline const SharedPtr<BuiltinType> BOOLEAN_TYPE = makeShared<BuiltinType>(TypeKind::Boolean);
    static inline const SharedPtr<BuiltinType> INTEGER_TYPE = makeShared<BuiltinType>(TypeKind::Integer);
    static inline const SharedPtr<BuiltinType> STRING_TYPE = makeShared<BuiltinType>(TypeKind::String);
    static inline const SharedPtr<BuiltinType> UNKNOWN_TYPE = makeShared<BuiltinType>(TypeKind::Unknown);
};

#endif //STATICSCRIPT_AST_TYPE_H
