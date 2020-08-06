#include "AST/Stmt.h"
#include "AST/Decl.h"

void VarDeclStmt::pushVarDecl(SharedPtr<VarDecl> varDecl) {
    childVarDecls.push_back(varDecl);
}

void VarDeclStmt::codegen() {

}
