#include "Driver/Visitor.h"

Visitor::Visitor(String filename) : filename(std::move(filename)) {}

antlrcpp::Any Visitor::visitModule(StaticScriptParser::ModuleContext *ctx) {
    const SharedPtrVector<Stmt> &childStmts = visitStatements(ctx->statements());
    return makeShared<Module>(filename, childStmts);
}

antlrcpp::Any Visitor::visitStatements(StaticScriptParser::StatementsContext *ctx) {
    SharedPtrVector<Stmt> stmts;
    if (ctx) {
        for (auto &stmtCtx: ctx->statement()) {
            SharedPtr<Stmt> stmt= visitStatement(stmtCtx);
            stmts.push_back(stmt);
        }
    }
    return stmts;
}

antlrcpp::Any Visitor::visitStatement(StaticScriptParser::StatementContext *ctx) {
    antlrcpp::Any stmtAny;
    SharedPtr<Stmt> stmt;
    if (ctx->emptyStatement()) {
        stmtAny =  visitEmptyStatement(ctx->emptyStatement());
    } else if (ctx->declarationStatement()) {
        stmtAny =  visitDeclarationStatement(ctx->declarationStatement());
        if (stmtAny.is<SharedPtr<VarDeclStmt>>()) {
            stmt = stmtAny.as<SharedPtr<VarDeclStmt>>();
        } else if (stmtAny.is<SharedPtr<FunctionDeclStmt>>()) {
            stmt = stmtAny.as<SharedPtr<FunctionDeclStmt>>();
        }
    } else if (ctx->compoundStatement()) {
        stmtAny =  visitCompoundStatement(ctx->compoundStatement());
        if (stmtAny.is<SharedPtr<CompoundStmt>>()) {
            stmt = stmtAny.as<SharedPtr<CompoundStmt>>();
        }
    } else if (ctx->expressionStatement()) {
        stmtAny =  visitExpressionStatement(ctx->expressionStatement());
        if (stmtAny.is<SharedPtr<Expr>>()) {
            stmt = stmtAny.as<SharedPtr<Expr>>();
        }
    } else if (ctx->selectionStatement()) {
        stmtAny =  visitSelectionStatement(ctx->selectionStatement());
        if (stmtAny.is<SharedPtr<IfStmt>>()) {
            stmt = stmtAny.as<SharedPtr<IfStmt>>();
        }
    } else if (ctx->iterationStatement()) {
        stmtAny =  visitIterationStatement(ctx->iterationStatement());
        if (stmtAny.is<SharedPtr<WhileStmt>>()) {
            stmt = stmtAny.as<SharedPtr<WhileStmt>>();
        } else if (stmtAny.is<SharedPtr<ForStmt>>()) {
            stmt = stmtAny.as<SharedPtr<ForStmt>>();
        }
    } else if (ctx->jumpStatement()) {
        stmtAny =  visitJumpStatement(ctx->jumpStatement());
        if (stmtAny.is<SharedPtr<ContinueStmt>>()) {
            stmt = stmtAny.as<SharedPtr<ContinueStmt>>();
        } else if (stmtAny.is<SharedPtr<BreakStmt>>()) {
            stmt = stmtAny.as<SharedPtr<BreakStmt>>();
        } else if (stmtAny.is<SharedPtr<ReturnStmt>>()) {
            stmt = stmtAny.as<SharedPtr<ReturnStmt>>();
        }
    }
    return stmt;
}

antlrcpp::Any Visitor::visitEmptyStatement(StaticScriptParser::EmptyStatementContext *ctx) {
    return nullptr;
}

antlrcpp::Any Visitor::visitDeclarationStatement(StaticScriptParser::DeclarationStatementContext *ctx) {
    return visitDeclaration(ctx->declaration());
}

antlrcpp::Any Visitor::visitDeclaration(StaticScriptParser::DeclarationContext *ctx) {
    if (ctx->variableDeclaration()) {
        return visitVariableDeclaration(ctx->variableDeclaration());
    } else if (ctx->functionDeclaration()) {
        return visitFunctionDeclaration(ctx->functionDeclaration());
    }
    return nullptr;
}

antlrcpp::Any Visitor::visitVariableDeclaration(StaticScriptParser::VariableDeclarationContext *ctx) {
    return visitVariableDeclarators(ctx->variableDeclarators());
}

antlrcpp::Any Visitor::visitVariableDeclarators(StaticScriptParser::VariableDeclaratorsContext *ctx) {
    VarModifier modifier = visitVariableModifier(ctx->variableModifier());
    SharedPtr<VarDeclStmt> varDeclStmt = makeShared<VarDeclStmt>();
    for (auto &declCtx : ctx->variableDeclarator()) {
        SharedPtr<VarDecl> varDecl = visitVariableDeclarator(declCtx);
        varDecl->modifier = modifier;
        varDeclStmt->pushVarDecl(varDecl);
    }
    return varDeclStmt;
}

antlrcpp::Any Visitor::visitVariableModifier(StaticScriptParser::VariableModifierContext *ctx) {
    if (ctx->Const()) {
        return VarModifier::Const;
    }
    return VarModifier::Let;
}

antlrcpp::Any Visitor::visitVariableDeclarator(StaticScriptParser::VariableDeclaratorContext *ctx) {
    SharedPtr<VarDecl> varDecl = makeShared<VarDecl>();
    varDecl->type = visitTypeAnnotation(ctx->typeAnnotation());
    varDecl->name = ctx->Identifier()->getText();
    varDecl->defaultVal = visitVariableInitializer(ctx->variableInitializer());
    return varDecl;
}

antlrcpp::Any Visitor::visitTypeAnnotation(StaticScriptParser::TypeAnnotationContext *ctx) {
    if (ctx) {
        return visitBuiltinType(ctx->builtinType());
    }
    return BuiltinType::UNKNOWN_TYPE;
}

antlrcpp::Any Visitor::visitBuiltinType(StaticScriptParser::BuiltinTypeContext *ctx) {
    if (ctx->Boolean()) {
        return BuiltinType::BOOLEAN_TYPE;
    } else if (ctx->Number()) {
        return BuiltinType::INTEGER_TYPE;
    }
    return BuiltinType::STRING_TYPE;
}

antlrcpp::Any Visitor::visitVariableInitializer(StaticScriptParser::VariableInitializerContext *ctx) {
    return visitExpression(ctx->expression());
}

antlrcpp::Any Visitor::visitFunctionDeclaration(StaticScriptParser::FunctionDeclarationContext *ctx) {
    String name = ctx->Identifier()->getText();
    SharedPtrVector<ParmVarDecl> params = visitParameterList(ctx->parameterList());
    SharedPtr<BuiltinType> returnType = visitTypeAnnotation(ctx->typeAnnotation());
    SharedPtr<CompoundStmt> body = visitFunctionBody(ctx->functionBody());
    SharedPtr<FunctionDecl> functionDecl = makeShared<FunctionDecl>(name, params, returnType, body);
    SharedPtr<FunctionDeclStmt> functionDeclStmt = makeShared<FunctionDeclStmt>(functionDecl);
    return functionDeclStmt;
}

antlrcpp::Any Visitor::visitParameterList(StaticScriptParser::ParameterListContext *ctx) {
    SharedPtrVector<ParmVarDecl> params;
    if (ctx) {
        for (size_t i = 0; i < ctx->Identifier().size(); ++i) {
            String name = ctx->Identifier(i)->getText();
            SharedPtr<BuiltinType> type = visitTypeAnnotation(ctx->typeAnnotation(i));
            SharedPtr<ParmVarDecl> param = makeShared<ParmVarDecl>(name, type);
            params.push_back(param);
        }
    }
    return params;
}

antlrcpp::Any Visitor::visitFunctionBody(StaticScriptParser::FunctionBodyContext *ctx) {
    return visitCompoundStatement(ctx->compoundStatement());
}

antlrcpp::Any Visitor::visitCallExpression(StaticScriptParser::CallExpressionContext *ctx) {
    String calleeName = ctx->Identifier()->getText();
    SharedPtrVector<Expr> args = visitArgumentList(ctx->argumentList());
    SharedPtr<CallExpr> callExpr = makeShared<CallExpr>(calleeName, args);
    return callExpr;
}

antlrcpp::Any Visitor::visitArgumentList(StaticScriptParser::ArgumentListContext *ctx) {
    return visitExpressionList(ctx->expressionList());
}

antlrcpp::Any Visitor::visitCompoundStatement(StaticScriptParser::CompoundStatementContext *ctx) {
    SharedPtrVector<Stmt> childStmts = visitStatements(ctx->statements());
    SharedPtr<CompoundStmt> compoundStmt = makeShared<CompoundStmt>(childStmts);
    return compoundStmt;
}

antlrcpp::Any Visitor::visitExpressionStatement(StaticScriptParser::ExpressionStatementContext *ctx) {
    return visitExpression(ctx->expression());
}

antlrcpp::Any Visitor::visitExpression(StaticScriptParser::ExpressionContext *ctx) {
    if (ctx) {
        SharedPtr<Expr> expr;
        if (ctx->literal()) {
            SharedPtr<LiteralExpr> literalExpr = visitLiteral(ctx->literal());
            expr = literalExpr;
        } else if (ctx->OpenParen()) {
            expr = visitExpression(ctx->expression(0));
        } else if (ctx->Identifier()) {
            expr = makeShared<IdentifierExpr>(ctx->Identifier()->getText());
        } else if (ctx->callExpression()) {
            SharedPtr<CallExpr> callExpr = visitCallExpression(ctx->callExpression());
            expr = callExpr;
        } else if (ctx->uop) {
            SharedPtr<Expr> subExpr = visitExpression(ctx->expression(0));
            expr = makeShared<UnaryOperatorExpr>(ctx->uop->getType(), subExpr);
        } else {
            SharedPtr<Expr> lhs = visitExpression(ctx->expression(0));
            SharedPtr<Expr> rhs = visitExpression(ctx->expression(1));
            expr = makeShared<BinaryOperatorExpr>(ctx->bop->getType(), lhs, rhs);
        }
        return expr;
    }
    return nullptr;
}

antlrcpp::Any Visitor::visitLiteral(StaticScriptParser::LiteralContext *ctx) {
    SharedPtr<LiteralExpr> literalExpr;
    if (ctx->BooleanLiteral()) {
        bool literal = ctx->BooleanLiteral()->getText() == "true";
        literalExpr = makeShared<BooleanLiteralExpr>(TypeKind::Boolean, literal);
    } else if (ctx->IntegerLiteral()) {
        int literal = std::stoi(ctx->IntegerLiteral()->getText());
        literalExpr = makeShared<IntegerLiteralExpr>(TypeKind::Integer, literal);
    } else {
        String literal = ctx->StringLiteral()->getText();
        literal = literal.substr(1, literal.size() - 2);
        literalExpr = makeShared<StringLiteralExpr>(TypeKind::String, literal);
    }
    return literalExpr;
}

antlrcpp::Any Visitor::visitSelectionStatement(StaticScriptParser::SelectionStatementContext *ctx) {
    if (ctx->ifStatement()) {
        return visitIfStatement(ctx->ifStatement());
    }
    return nullptr;
}

antlrcpp::Any Visitor::visitIfStatement(StaticScriptParser::IfStatementContext *ctx) {
    SharedPtr<Expr> condition = visitExpression(ctx->expression());
    SharedPtr<Stmt> thenBody = visitStatement(ctx->statement(0));
    SharedPtr<Stmt> elseBody;
    if (ctx->Else()) {
        elseBody = visitStatement(ctx->statement(1));
    }
    SharedPtr<IfStmt> ifStmt = makeShared<IfStmt>(condition, thenBody, elseBody);
    return ifStmt;
}

antlrcpp::Any Visitor::visitIterationStatement(StaticScriptParser::IterationStatementContext *ctx) {
    if (ctx->whileStatement()) {
        return visitWhileStatement(ctx->whileStatement());
    } else if (ctx->forStatement()) {
        return visitForStatement(ctx->forStatement());
    }
    return nullptr;
}

antlrcpp::Any Visitor::visitWhileStatement(StaticScriptParser::WhileStatementContext *ctx) {
    SharedPtr<Expr> condition = visitExpression(ctx->expression());
    SharedPtr<Stmt> body = visitStatement(ctx->statement());
    SharedPtr<WhileStmt> whileStmt = makeShared<WhileStmt>(condition, body);
    return whileStmt;
}

antlrcpp::Any Visitor::visitForStatement(StaticScriptParser::ForStatementContext *ctx) {
    antlrcpp::Any forInitAny = visitForInit(ctx->forInit());
    SharedPtr<VarDeclStmt> forInitVarDecls;
    SharedPtrVector<Expr> forInitExprList;
    if (forInitAny.isNotNull()) {
        if (forInitAny.is<SharedPtr<VarDeclStmt>>()) {
            forInitVarDecls = forInitAny.as<SharedPtr<VarDeclStmt>>();
        } else if (forInitAny.is<SharedPtrVector<Expr>>()) {
            forInitExprList = forInitAny.as<SharedPtrVector<Expr>>();
        }
    }
    SharedPtr<Expr> forCondition = visitExpression(ctx->forCondition);
    SharedPtrVector<Expr> forUpdate = visitExpressionList(ctx->forUpdate);
    SharedPtr<Stmt> body = visitStatement(ctx->statement());
    SharedPtr<ForStmt> forStmt = makeShared<ForStmt>(forInitVarDecls, forInitExprList, forCondition, forUpdate, body);
    return forStmt;
}

antlrcpp::Any Visitor::visitForInit(StaticScriptParser::ForInitContext *ctx) {
    if (ctx) {
        if (ctx->variableDeclarators()) {
            return visitVariableDeclarators(ctx->variableDeclarators());
        } else if (ctx->expressionList()) {
            return visitExpressionList(ctx->expressionList());
        }
    }
    return nullptr;
}

antlrcpp::Any Visitor::visitExpressionList(StaticScriptParser::ExpressionListContext *ctx) {
    SharedPtrVector<Expr> exprList;
    for (auto exprCtx : ctx->expression()) {
        SharedPtr<Expr> expr = visitExpression(exprCtx);
        exprList.push_back(expr);
    }
    return exprList;
}

antlrcpp::Any Visitor::visitJumpStatement(StaticScriptParser::JumpStatementContext *ctx) {
    if (ctx->continueStatement()) {
        return visitContinueStatement(ctx->continueStatement());
    } else if (ctx->breakStatement()) {
        return visitBreakStatement(ctx->breakStatement());
    } else if (ctx->returnStatement()) {
        return visitReturnStatement(ctx->returnStatement());
    }
    return nullptr;
}

antlrcpp::Any Visitor::visitContinueStatement(StaticScriptParser::ContinueStatementContext *ctx) {
    SharedPtr<ContinueStmt> continueStmt = makeShared<ContinueStmt>();
    return continueStmt;
}

antlrcpp::Any Visitor::visitBreakStatement(StaticScriptParser::BreakStatementContext *ctx) {
    SharedPtr<BreakStmt> breakStmt = makeShared<BreakStmt>();
    return breakStmt;
}

antlrcpp::Any Visitor::visitReturnStatement(StaticScriptParser::ReturnStatementContext *ctx) {
    SharedPtr<Expr> argument = visitExpression(ctx->expression());
    SharedPtr<ReturnStmt> returnStmt = makeShared<ReturnStmt>(argument);
    return returnStmt;
}
