#ifndef STATICSCRIPT_AST_MODULE_H
#define STATICSCRIPT_AST_MODULE_H

#include "AST/StmtNode.h"

/// 一个文件就是一个模块, 即一个翻译单元
class Module final {
public:
    virtual ~Module() = default;

    explicit Module(String filename, SharedPtrVector<StmtNode> childStmts);

    [[nodiscard]] virtual const String &getFilename() const;

    [[nodiscard]] virtual bool isEmpty() const;

    String filename;
    SharedPtrVector<StmtNode> childStmts;
};


#endif // STATICSCRIPT_AST_MODULE_H
