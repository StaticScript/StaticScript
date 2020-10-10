#pragma once

#include <iostream>
#include "Util/Alias.h"

template<typename T>
static inline void print(std::ostream &outputStream, T &&content) {
    outputStream << content;
}

template<typename T, typename ...U>
static inline void print(std::ostream &outputStream, T &&content, U &&...restContent) {
    outputStream << content;
    print(outputStream, restContent...);
}

template<typename ...T>
inline void outPrint(T &&...content) {
    print(std::cout, content...);
}

inline void outPrintln() {
    std::cout << std::endl;
}

template<typename ...T>
inline void outPrintln(T &&...content) {
    print(std::cout, content...);
    outPrintln();
}

template<typename ...T>
inline void errPrint(T &&...content) {
    print(std::cerr, content...);
}

inline void errPrintln() {
    std::cerr << std::endl;
}

template<typename ...T>
inline void errPrintln(T &&...content) {
    print(std::cerr, content...);
    errPrintln();
}
