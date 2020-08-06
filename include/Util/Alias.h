#ifndef STATICSCRIPT_UTIL_ALIAS_H
#define STATICSCRIPT_UTIL_ALIAS_H

#include <memory>
#include <vector>

template <typename T>
using SharedPtr = std::shared_ptr<T>;

template <typename T>
using SharedPtrVector = std::vector<SharedPtr<T>>;

template <typename T, typename ...Args>
SharedPtr<T> makeShared(Args&& ...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#endif // STATICSCRIPT_UTIL_ALIAS_H