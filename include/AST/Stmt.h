#ifndef STATICSCRIPT_AST_STMT_H
#define STATICSCRIPT_AST_STMT_H

#include "AST/Base.h"

class VarDecl;

class Stmt : public ASTNode {
public:
    ~Stmt() override = default;
};

// 值语句
class ValueStmt : public Stmt {
public:
    ~ValueStmt() override = default;
};

// 复合语句
class CompoundStmt : public Stmt {
public:
    ~CompoundStmt() override = default;
};

// 声明语句
class VarDeclStmt : public Stmt {
public:
    ~VarDeclStmt() override = default;
    virtual void pushVarDecl(SharedPtr<VarDecl> varDecl);

    void codegen() override;

private:
    SharedPtrVector<VarDecl> childVarDecls;
};

// if语句
class IfStmt : public Stmt {
public:
    ~IfStmt() override = default;
};

// while语句
class WhileStmt : public Stmt {
public:
    ~WhileStmt() override = default;
};

// for语句
class ForStmt : public Stmt {
public:
    ~ForStmt() override = default;
};

// continue语句
class ContinueStmt : public Stmt {
public:
    ~ContinueStmt() override = default;
};

// break语句
class BreakStmt : public Stmt {
public:
    ~BreakStmt() override = default;
};

// return语句
class ReturnStmt : public Stmt {
public:
    ~ReturnStmt() override = default;
};


#endif //STATICSCRIPT_AST_STMT_H
