#pragma once

#include "Support/Alias.h"
#include "Support/Error.h"

/// 原子类型的种类, Unknown只用于空数组的元素类型
enum class AtomicTypeKind {
    Boolean, Integer, String, Unknown
};

/// 类型节点
class Type {
public:
    virtual ~Type() = default;

    [[nodiscard]] virtual bool isAtomic() const = 0;

    [[nodiscard]] virtual bool isArray() const = 0;

    [[nodiscard]] virtual bool isUnknown() const = 0;

    [[nodiscard]] bool equals(const SharedPtr<Type> &rhs) const;
};

/// 原子类型节点
class AtomicType : public Type {
public:
    explicit AtomicType(AtomicTypeKind kind);

    explicit AtomicType() = default;

    ~AtomicType() override = default;

    [[nodiscard]] bool isAtomic() const override;

    [[nodiscard]] bool isArray() const override;

    [[nodiscard]] bool isUnknown() const override;

    [[nodiscard]] AtomicTypeKind getKind() const;

    bool operator==(const AtomicType &rhs) = delete;

    bool operator!=(const AtomicType &rhs) = delete;

    [[nodiscard]] bool equals(const SharedPtr<AtomicType> &rhs) const;

    static inline const SharedPtr<AtomicType> BOOLEAN_TYPE = makeShared<AtomicType>(AtomicTypeKind::Boolean); // NOLINT
    static inline const SharedPtr<AtomicType> INTEGER_TYPE = makeShared<AtomicType>(AtomicTypeKind::Integer); // NOLINT
    static inline const SharedPtr<AtomicType> STRING_TYPE = makeShared<AtomicType>(AtomicTypeKind::String); // NOLINT
    static inline const SharedPtr<AtomicType> UNKNOWN_TYPE = makeShared<AtomicType>(AtomicTypeKind::Unknown); // NOLINT
private:
    AtomicTypeKind kind = AtomicTypeKind::Unknown;
};

/// 数组类型节点
class ArrayType : public Type {
public:
    static SharedPtr<ArrayType> createNDArrayType(const SharedPtr<AtomicType> &atomicType, size_t depth);

    explicit ArrayType(const SharedPtr<Type> &elementType);

    explicit ArrayType() = default;

    ~ArrayType() override = default;

    [[nodiscard]] bool isAtomic() const override;

    [[nodiscard]] bool isArray() const override;

    [[nodiscard]] bool isUnknown() const override;

    [[nodiscard]] const SharedPtr<Type> &getElementType() const;

    [[nodiscard]] size_t getDepth() const;

    void setElementType(const SharedPtr<Type> &eleType);

    bool operator==(const ArrayType &rhs) = delete;

    bool operator!=(const ArrayType &rhs) = delete;

    [[nodiscard]] bool equals(const SharedPtr<ArrayType> &rhs) const;
private:
    SharedPtr<Type> elementType = AtomicType::UNKNOWN_TYPE;
    size_t depth = 1;
};