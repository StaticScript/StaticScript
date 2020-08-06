#ifndef STATICSCRIPT_AST_BASE_H
#define STATICSCRIPT_AST_BASE_H

#include <memory>

template <typename T>
using SharedPtr = std::shared_ptr<T>;

template <typename T>
using SharedPtrVector = std::vector<SharedPtr<T>>;

template <typename T, typename ...Args>
SharedPtr<T> makeShared(Args&& ...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

class ASTNode {
public:
    virtual ~ASTNode() = default;

    virtual void codegen() = 0;
};


#endif //STATICSCRIPT_AST_BASE_H
