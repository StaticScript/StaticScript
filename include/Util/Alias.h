#ifndef STATICSCRIPT_UTIL_ALIAS_H
#define STATICSCRIPT_UTIL_ALIAS_H

#include <string>
#include <memory>
#include <vector>

using String = std::string;

template <typename T>
using SharedPtr = std::shared_ptr<T>;

template <typename T>
using SharedPtrVector = std::vector<SharedPtr<T>>;

template<typename T, typename ...Args>
inline SharedPtr<T> makeShared(Args &&...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#endif // STATICSCRIPT_UTIL_ALIAS_H