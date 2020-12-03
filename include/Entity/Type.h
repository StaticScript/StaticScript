#pragma once

#include <memory>
#include "Support/Alias.h"
#include "Support/Error.h"

/// 基础类型的种类, Unknown只用于空数组的元素类型
enum class BasicTypeKind {
    Boolean, Integer, Float, String, Unknown
};

class ArrayType;

/// 类型节点
class Type : public std::enable_shared_from_this<Type> {
public:
    virtual ~Type() = default;

    [[nodiscard]] virtual bool isBasic() const = 0;

    [[nodiscard]] virtual bool isBoolean() const = 0;

    [[nodiscard]] virtual bool isInteger() const = 0;

    [[nodiscard]] virtual bool isFloat() const = 0;

    [[nodiscard]] virtual bool isNumber() const = 0;

    [[nodiscard]] virtual bool isString() const = 0;

    [[nodiscard]] virtual bool isUnknown() const = 0;

    [[nodiscard]] virtual bool isArray() const = 0;

    [[nodiscard]] virtual SharedPtr<ArrayType> asArray() const = 0;

    bool operator==(const Type &rhs) = delete;

    bool operator!=(const Type &rhs) = delete;

    /// 判断类型是否相等[无方向](note: equals < sameAs < compatible)
    [[nodiscard]] bool equals(const SharedPtr<Type> &rhs) const;

    /// 判断类型是否相同[无方向](note: equals < sameAs < compatible)
    [[nodiscard]] bool sameAs(const SharedPtr<Type> &rhs) const;

    /// 判断类型是否兼容[有方向](note: equals < sameAs < compatible)
    [[nodiscard]] bool compatible(const SharedPtr<Type> &rhs) const;
};

/// 基础类型节点
class BasicType : public Type {
public:
    explicit BasicType(BasicTypeKind kind);

    explicit BasicType() = default;

    ~BasicType() override = default;

    [[nodiscard]] bool isBasic() const override;

    [[nodiscard]] bool isBoolean() const override;

    [[nodiscard]] bool isInteger() const override;

    [[nodiscard]] bool isFloat() const override;

    [[nodiscard]] bool isNumber() const override;

    [[nodiscard]] bool isString() const override;

    [[nodiscard]] bool isUnknown() const override;

    [[nodiscard]] bool isArray() const override;

    [[nodiscard]] SharedPtr<ArrayType> asArray() const override;

    [[nodiscard]] BasicTypeKind getKind() const;

    bool operator==(const BasicType &rhs) = delete;

    bool operator!=(const BasicType &rhs) = delete;

    [[nodiscard]] bool equals(const SharedPtr<BasicType> &rhs) const;

    static inline const SharedPtr<BasicType> BOOLEAN_TYPE = makeShared<BasicType>(BasicTypeKind::Boolean);   // NOLINT
    static inline const SharedPtr<BasicType> INTEGER_TYPE = makeShared<BasicType>(BasicTypeKind::Integer);   // NOLINT
    static inline const SharedPtr<BasicType> FLOAT_TYPE = makeShared<BasicType>(BasicTypeKind::Float);       // NOLINT
    static inline const SharedPtr<BasicType> STRING_TYPE = makeShared<BasicType>(BasicTypeKind::String);     // NOLINT
    static inline const SharedPtr<BasicType> UNKNOWN_TYPE = makeShared<BasicType>(BasicTypeKind::Unknown);   // NOLINT
private:
    BasicTypeKind kind = BasicTypeKind::Unknown;
};

/// 数组类型节点
class ArrayType : public Type {
public:
    static SharedPtr<ArrayType> createNDArrayType(const SharedPtr<BasicType> &basicType, size_t depth);

    explicit ArrayType(const SharedPtr<Type> &elementType);

    explicit ArrayType() = default;

    ~ArrayType() override = default;

    [[nodiscard]] bool isBasic() const override;

    [[nodiscard]] bool isBoolean() const override;

    [[nodiscard]] bool isInteger() const override;

    [[nodiscard]] bool isFloat() const override;

    [[nodiscard]] bool isNumber() const override;

    [[nodiscard]] bool isString() const override;

    [[nodiscard]] bool isUnknown() const override;

    [[nodiscard]] bool isArray() const override;

    [[nodiscard]] SharedPtr<ArrayType> asArray() const override;

    /// 获取数组的元素类型, 多维数组则获取子数组的类型
    [[nodiscard]] const SharedPtr<Type> &getElementType() const;

    /// 获取多维数组最内层的元素类型
    [[nodiscard]] SharedPtr<Type> getBasicElementType() const;

    [[nodiscard]] size_t getDepth() const;

    bool operator==(const ArrayType &rhs) = delete;

    bool operator!=(const ArrayType &rhs) = delete;

    [[nodiscard]] bool equals(const SharedPtr<ArrayType> &rhs) const;

private:
    SharedPtr<Type> elementType = BasicType::UNKNOWN_TYPE;
    size_t depth = 1;
};