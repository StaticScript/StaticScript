#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>

using String = std::string;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
using SharedPtrVector = std::vector<SharedPtr<T>>;

template<typename K, typename V>
using SharedPtrMap = std::map<K, SharedPtr<V>>;

template<typename T, typename ...Args>
inline SharedPtr<T> makeShared(Args &&...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename P>
inline SharedPtr<T> staticPtrCast(const SharedPtr<P> &ptr) {
    return std::static_pointer_cast<T>(ptr);
}

template<typename T, typename P>
inline SharedPtr<T> dynPtrCast(const SharedPtr<P> &ptr) {
    return std::dynamic_pointer_cast<T>(ptr);
}
