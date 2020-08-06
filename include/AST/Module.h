#ifndef STATICSCRIPT_AST_MODULE_H
#define STATICSCRIPT_AST_MODULE_H

#include "AST/Stmt.h"

// 模块声明
class Module {
public:
    ~Module() = default;

    explicit Module(String filename);

    explicit Module(String filename, SharedPtrVector<Stmt> childStmts);

    [[nodiscard]] const String &getFilename() const;

    [[nodiscard]] bool isEmpty() const;

    void codegen();

private:
    const String filename;
    bool empty;
    SharedPtrVector<Stmt> childStmts;
};


#endif //STATICSCRIPT_AST_MODULE_H
