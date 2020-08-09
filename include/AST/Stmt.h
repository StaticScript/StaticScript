#ifndef STATICSCRIPT_AST_STMT_H
#define STATICSCRIPT_AST_STMT_H

#include "Util/Alias.h"

class VarDecl;

class FunctionDecl;

class Expr;

class Stmt {
public:
    virtual ~Stmt() = default;
};

// 值语句
class ValueStmt : public Stmt {
public:
    ~ValueStmt() override = default;
};

// 复合语句
class CompoundStmt : public Stmt {
public:

    explicit CompoundStmt(const SharedPtrVector<Stmt> &childStmts);

    ~CompoundStmt() override = default;

    SharedPtrVector<Stmt> childStmts;
};

// 变量声明语句
class VarDeclStmt : public Stmt {
public:
    ~VarDeclStmt() override = default;

    virtual void pushVarDecl(const SharedPtr<VarDecl> &varDecl);

private:
    SharedPtrVector<VarDecl> childVarDecls;
};

// 函数声明语句
class FunctionDeclStmt : public Stmt {
public:
    explicit FunctionDeclStmt(const SharedPtr<FunctionDecl> &childFunctionDecl);

    ~FunctionDeclStmt() override = default;

private:
    SharedPtr<FunctionDecl> childFunctionDecl;
};

// if语句
class IfStmt : public Stmt {
public:
    IfStmt(const SharedPtr<Expr> &condition, const SharedPtr<Stmt> &thenStmt, const SharedPtr<Stmt> &elseStmt);

    ~IfStmt() override = default;

    SharedPtr<Expr> condition;
    SharedPtr<Stmt> thenBody;
    SharedPtr<Stmt> elseBody;
};

// while语句
class WhileStmt : public Stmt {
public:
    WhileStmt(const SharedPtr<Expr> &condition, const SharedPtr<Stmt> &body);

    ~WhileStmt() override = default;

    SharedPtr<Expr> condition;
    SharedPtr<Stmt> body;
};

// for语句
class ForStmt : public Stmt {
public:
    ForStmt(const SharedPtr<VarDeclStmt> &forInitVarDecls, const SharedPtrVector<Expr> &forInitExprList, const SharedPtr<Expr> &forCondition,
            const SharedPtrVector<Expr> &forUpdate, const SharedPtr<Stmt> &body);

    ~ForStmt() override = default;

    SharedPtr<VarDeclStmt> forInitVarDecls;
    SharedPtrVector<Expr> forInitExprList;
    SharedPtr<Expr> forCondition;
    SharedPtrVector<Expr> forUpdate;
    SharedPtr<Stmt> body;
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
    explicit ReturnStmt(const SharedPtr<Expr> &argument);

    ~ReturnStmt() override = default;

    SharedPtr<Expr> argument;
};


#endif //STATICSCRIPT_AST_STMT_H
