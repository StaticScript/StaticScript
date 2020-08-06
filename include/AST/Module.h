#ifndef STATICSCRIPT_AST_MODULE_H
#define STATICSCRIPT_AST_MODULE_H

#include <vector>
#include <string>
#include "AST/Stmt.h"

// 模块声明
class Module {
public:
    ~Module() = default;

    explicit Module(std::string filename);

    explicit Module(std::string filename, SharedPtrVector<Stmt> childStmts);

    [[nodiscard]] const std::string &getFilename() const;

    [[nodiscard]] bool isEmpty() const;

    void codegen();

private:
    const std::string filename;
    bool empty;
    SharedPtrVector<Stmt> childStmts;
};


#endif //STATICSCRIPT_AST_MODULE_H
