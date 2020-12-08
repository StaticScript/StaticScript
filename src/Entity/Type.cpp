#include "Entity/Type.h"

bool Type::equals(const SharedPtr<Type> &rhs) const {
    // 如果rhs为nullptr, 直接返回false
    if (!rhs) {
        return false;
    }
    // 如果比较的两类型不属于同类, 直接返回false
    if (isBasic() && rhs->isArray() || isArray() && rhs->isBasic()) {
        return false;
    }
    if (SharedPtr<BasicType> basicType = dynPtrCast<BasicType>(rhs)) {
        const auto *type = dynamic_cast<const BasicType *>(this);
        return type->equals(basicType);
    } else {
        SharedPtr<ArrayType> arrayType = dynPtrCast<ArrayType>(rhs);
        const auto *type = dynamic_cast<const ArrayType *>(this);
        return type->equals(arrayType);
    }
}

// 相同类型
// 1. 相等的类型是相同类型
// 2. 基础整型和基础浮点型是相同类型
// 3. 数组与空数组是相同类型
bool Type::sameAs(const SharedPtr<Type> &rhs) const {
    // 右操作子为空直接返回false
    if (!rhs) {
        return false;
    }
    // 相等的类型是兼容的
    if (equals(rhs)) {
        return true;
    }
    // 整数和浮点数是同类的
    if (isNumber() && rhs->isNumber()) {
        return true;
    }
    if (isArray() && rhs->isUnknown()) {
        // 1. 左操作子为数组, 右操作子为unknown基础类型, 左操作子兼容右操作子(反之不成立)
        // 2. 左右操作子都是数组, 且右操作子为空树组, 左操作子深度不小于右操作子深度, 左操作子兼容右操作子(反之不成立)
        if (rhs->isBasic() || rhs->isArray() && asArray()->getDepth() >= rhs->asArray()->getDepth()) {
            return true;
        }
    }
    return false;
}

// 左操作子: 形式上的类型(左值的类型(变量声明类型), 函数形参类型)
// 右操作子: 实际的类型(右值的类型(变量赋值类型), 函数实参类型)
// 兼容是有方向的, 左操作子兼容右操作子并不一定代表右操作子兼容左操作子
bool Type::compatibleWith(const SharedPtr<Type> &rhs) const {
    // 右操作子为空直接返回false
    if (!rhs) {
        return false;
    }
    // 同类的类型是兼容的
    if (equals(rhs) || sameAs(rhs)) {
        return true;
    }
    if (isNumberArray() && rhs->isNumberArray()) {
        if (asArray()->getDepth() == rhs->asArray()->getDepth()) {
            return true;
        }
    }
    return false;
}

BasicType::BasicType(BasicTypeKind kind) : kind(kind) {}

bool BasicType::isBasic() const {
    return true;
}

bool BasicType::isBoolean() const {
    return kind == BasicTypeKind::Boolean;
}

bool BasicType::isInteger() const {
    return kind == BasicTypeKind::Integer;
}

bool BasicType::isFloat() const {
    return kind == BasicTypeKind::Float;
}

bool BasicType::isNumber() const {
    return kind == BasicTypeKind::Integer || kind == BasicTypeKind::Float;
}

bool BasicType::isString() const {
    return kind == BasicTypeKind::String;
}

bool BasicType::isUnknown() const {
    return kind == BasicTypeKind::Unknown;
}

bool BasicType::isArray() const {
    return false;
}

SharedPtr<ArrayType> BasicType::asArray() const {
    return nullptr;
}

BasicTypeKind BasicType::getKind() const {
    return kind;
}

bool BasicType::equals(const SharedPtr<BasicType> &rhs) const {
    return kind == rhs->kind;
}

bool BasicType::isNumberArray() const {
    return false;
}

SharedPtr<ArrayType> ArrayType::createNDArrayType(const SharedPtr<BasicType> &basicType, size_t depth) {
    SharedPtr<Type> type = basicType;
    for (size_t i = 0; i < depth; ++i) {
        type = makeShared<ArrayType>(type);
    }
    return staticPtrCast<ArrayType>(type);
}

ArrayType::ArrayType(const SharedPtr<Type> &elementType) : elementType(elementType) {
    if (elementType->isArray()) {
        depth = elementType->asArray()->depth + 1;
    }
}

bool ArrayType::isBasic() const {
    return false;
}

bool ArrayType::isBoolean() const {
    return false;
}

bool ArrayType::isInteger() const {
    return false;
}

bool ArrayType::isFloat() const {
    return false;
}

bool ArrayType::isNumber() const {
    return false;
}

bool ArrayType::isString() const {
    return false;
}

bool ArrayType::isUnknown() const {
    return getBasicElementType()->isUnknown();
}

bool ArrayType::isArray() const {
    return true;
}

SharedPtr<ArrayType> ArrayType::asArray() const {
    return constPtrCast<ArrayType>(staticPtrCast<const ArrayType>(shared_from_this()));
}

const SharedPtr<Type> &ArrayType::getElementType() const {
    return elementType;
}

SharedPtr<Type> ArrayType::getBasicElementType() const {
    if (elementType->isBasic()) {
        return elementType;
    }
    SharedPtr<Type> iterType = elementType;
    while (iterType->isArray()) {
        iterType = iterType->asArray()->elementType;
    }
    return iterType;
}

bool ArrayType::equals(const SharedPtr<ArrayType> &rhs) const {
    return elementType->equals(rhs->elementType);
}

size_t ArrayType::getDepth() const {
    return depth;
}

bool ArrayType::isNumberArray() const {
    if (getBasicElementType()->isNumber()) {
        return true;
    }
    return false;
}
