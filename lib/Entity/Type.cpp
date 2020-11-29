#include "Entity/Type.h"

bool Type::equals(const SharedPtr<Type> &rhs) const {
    // 如果rhs为nullptr, 直接返回false
    if (!rhs) {
        return false;
    }
    // 如果比较的两类型不属于同类, 直接返回false
    if (isAtomic() && rhs->isArray() || isArray() && rhs->isAtomic()) {
        return false;
    }
    if (SharedPtr<AtomicType> atomicType = dynPtrCast<AtomicType>(rhs)) {
        const auto *type = dynamic_cast<const AtomicType *>(this);
        return type->equals(atomicType);
    } else {
        SharedPtr<ArrayType> arrayType = dynPtrCast<ArrayType>(rhs);
        const auto *type = dynamic_cast<const ArrayType *>(this);
        return type->equals(arrayType);
    }
}

AtomicType::AtomicType(AtomicTypeKind kind) : kind(kind) {}

bool AtomicType::isAtomic() const {
    return true;
}

bool AtomicType::isArray() const {
    return false;
}

bool AtomicType::isUnknown() const {
    return kind == AtomicTypeKind::Unknown;
}

AtomicTypeKind AtomicType::getKind() const {
    return kind;
}

bool AtomicType::equals(const SharedPtr<AtomicType> &rhs) const {
    return kind == rhs->kind;
}

SharedPtr<ArrayType> ArrayType::createNDArrayType(const SharedPtr<AtomicType> &atomicType, size_t depth) {
    reportOnTypeError(depth == 0, "Dimension of multidimensional array must be positive");
    SharedPtr<Type> type = atomicType;
    for (size_t i = 0; i < depth; ++i) {
        type = makeShared<ArrayType>(type);
    }
    return staticPtrCast<ArrayType>(type);
}

ArrayType::ArrayType(const SharedPtr<Type> &elementType) : elementType(elementType) {
    if (elementType->isArray()) {
        depth = staticPtrCast<ArrayType>(elementType)->depth + 1;
    }
}

bool ArrayType::isAtomic() const {
    return false;
}

bool ArrayType::isArray() const {
    return true;
}

bool ArrayType::isUnknown() const {
    return elementType->isUnknown();
}

const SharedPtr<Type> &ArrayType::getElementType() const {
    return elementType;
}

void ArrayType::setElementType(const SharedPtr<Type> &eleType) {
    elementType = eleType;
}

bool ArrayType::equals(const SharedPtr<ArrayType> &rhs) const {
    return elementType->equals(rhs->elementType);
}

size_t ArrayType::getDepth() const {
    return depth;
}
