#include "Util/Alias.h"

template<typename K, typename V>
inline SharedPtr<V> mapFind(const SharedPtrMap<K, V> &map, const K &key) {
    auto iter = map.find(key);
    if (iter != map.end()) {
        return iter->second;
    }
    return nullptr;
}
