#include "AST/ASTBuilder.h"

ASTBuilder::ASTBuilder(String filename) : filename(std::move(filename)) {}

antlrcpp::Any ASTBuilder::visitModule(StaticScriptParser::ModuleContext *ctx) {
    const SharedPtrVector<StmtNode> &childStmts = visitStatements(ctx->statements());
    return makeShared<Module>(filename, childStmts);
}

antlrcpp::Any ASTBuilder::visitStatements(StaticScriptParser::StatementsContext *ctx) {
    SharedPtrVector<StmtNode> stmts;
    if (ctx) {
        for (auto &stmtCtx: ctx->statement()) {
            SharedPtr<StmtNode> stmt = visitStatement(stmtCtx);
            stmts.push_back(stmt);
        }
    }
    return stmts;
}

antlrcpp::Any ASTBuilder::visitStatement(StaticScriptParser::StatementContext *ctx) {
    antlrcpp::Any stmtAny;
    SharedPtr<StmtNode> stmt;
    if (ctx->emptyStatement()) {
        stmtAny = visitEmptyStatement(ctx->emptyStatement());
    } else if (ctx->declarationStatement()) {
        stmtAny = visitDeclarationStatement(ctx->declarationStatement());
        if (stmtAny.is<SharedPtr<VarDeclStmtNode>>()) {
            stmt = stmtAny.as<SharedPtr<VarDeclStmtNode>>();
        } else if (stmtAny.is<SharedPtr<FunctionDeclStmtNode>>()) {
            stmt = stmtAny.as<SharedPtr<FunctionDeclStmtNode>>();
        }
    } else if (ctx->compoundStatement()) {
        stmtAny = visitCompoundStatement(ctx->compoundStatement());
        if (stmtAny.is<SharedPtr<CompoundStmtNode>>()) {
            stmt = stmtAny.as<SharedPtr<CompoundStmtNode>>();
        }
    } else if (ctx->expressionStatement()) {
        stmtAny = visitExpressionStatement(ctx->expressionStatement());
        if (stmtAny.is<SharedPtr<ExprStmtNode>>()) {
            stmt = stmtAny.as<SharedPtr<ExprStmtNode>>();
        }
    } else if (ctx->selectionStatement()) {
        stmtAny = visitSelectionStatement(ctx->selectionStatement());
        if (stmtAny.is<SharedPtr<IfStmtNode>>()) {
            stmt = stmtAny.as<SharedPtr<IfStmtNode>>();
        }
    } else if (ctx->iterationStatement()) {
        stmtAny = visitIterationStatement(ctx->iterationStatement());
        if (stmtAny.is<SharedPtr<WhileStmtNode>>()) {
            stmt = stmtAny.as<SharedPtr<WhileStmtNode>>();
        } else if (stmtAny.is<SharedPtr<ForStmtNode>>()) {
            stmt = stmtAny.as<SharedPtr<ForStmtNode>>();
        }
    } else if (ctx->jumpStatement()) {
        stmtAny = visitJumpStatement(ctx->jumpStatement());
        if (stmtAny.is<SharedPtr<ContinueStmtNode>>()) {
            stmt = stmtAny.as<SharedPtr<ContinueStmtNode>>();
        } else if (stmtAny.is<SharedPtr<BreakStmtNode>>()) {
            stmt = stmtAny.as<SharedPtr<BreakStmtNode>>();
        } else if (stmtAny.is<SharedPtr<ReturnStmtNode>>()) {
            stmt = stmtAny.as<SharedPtr<ReturnStmtNode>>();
        }
    }
    return stmt;
}

antlrcpp::Any ASTBuilder::visitEmptyStatement(StaticScriptParser::EmptyStatementContext *ctx) {
    return nullptr;
}

antlrcpp::Any ASTBuilder::visitDeclarationStatement(StaticScriptParser::DeclarationStatementContext *ctx) {
    return visitDeclaration(ctx->declaration());
}

antlrcpp::Any ASTBuilder::visitDeclaration(StaticScriptParser::DeclarationContext *ctx) {
    if (ctx->variableDeclaration()) {
        return visitVariableDeclaration(ctx->variableDeclaration());
    } else if (ctx->functionDeclaration()) {
        return visitFunctionDeclaration(ctx->functionDeclaration());
    }
    return nullptr;
}

antlrcpp::Any ASTBuilder::visitVariableDeclaration(StaticScriptParser::VariableDeclarationContext *ctx) {
    return visitVariableDeclarators(ctx->variableDeclarators());
}

antlrcpp::Any ASTBuilder::visitVariableDeclarators(StaticScriptParser::VariableDeclaratorsContext *ctx) {
    VarModifier modifier = visitVariableModifier(ctx->variableModifier());
    SharedPtr<VarDeclStmtNode> varDeclStmt = makeShared<VarDeclStmtNode>();
    for (auto &declCtx : ctx->variableDeclarator()) {
        SharedPtr<VarDeclNode> varDecl = visitVariableDeclarator(declCtx);
        varDecl->modifier = modifier;
        varDeclStmt->pushVarDecl(varDecl);
    }
    return varDeclStmt;
}

antlrcpp::Any ASTBuilder::visitVariableModifier(StaticScriptParser::VariableModifierContext *ctx) {
    if (ctx->Const()) {
        return VarModifier::Const;
    }
    return VarModifier::Let;
}

antlrcpp::Any ASTBuilder::visitVariableDeclarator(StaticScriptParser::VariableDeclaratorContext *ctx) {
    SharedPtr<VarDeclNode> varDecl = makeShared<VarDeclNode>();
    varDecl->type = visitTypeAnnotation(ctx->typeAnnotation());
    varDecl->name = ctx->Identifier()->getText();
    varDecl->defaultVal = visitVariableInitializer(ctx->variableInitializer());
    return varDecl;
}

antlrcpp::Any ASTBuilder::visitTypeAnnotation(StaticScriptParser::TypeAnnotationContext *ctx) {
    if (ctx) {
        return visitBuiltinType(ctx->builtinType());
    }
    return BuiltinTypeNode::UNKNOWN_TYPE;
}

antlrcpp::Any ASTBuilder::visitBuiltinType(StaticScriptParser::BuiltinTypeContext *ctx) {
    if (ctx->Boolean()) {
        return BuiltinTypeNode::BOOLEAN_TYPE;
    } else if (ctx->Number()) {
        return BuiltinTypeNode::INTEGER_TYPE;
    }
    return BuiltinTypeNode::STRING_TYPE;
}

antlrcpp::Any ASTBuilder::visitVariableInitializer(StaticScriptParser::VariableInitializerContext *ctx) {
    return visitExpression(ctx->expression());
}

antlrcpp::Any ASTBuilder::visitFunctionDeclaration(StaticScriptParser::FunctionDeclarationContext *ctx) {
    String name = ctx->Identifier()->getText();
    SharedPtrVector<ParmVarDeclNode> params = visitParameterList(ctx->parameterList());
    SharedPtr<BuiltinTypeNode> returnType = visitTypeAnnotation(ctx->typeAnnotation());
    SharedPtr<CompoundStmtNode> body = visitFunctionBody(ctx->functionBody());
    SharedPtr<FunctionDeclNode> functionDecl = makeShared<FunctionDeclNode>(name, params, returnType, body);
    SharedPtr<FunctionDeclStmtNode> functionDeclStmt = makeShared<FunctionDeclStmtNode>(functionDecl);
    return functionDeclStmt;
}

antlrcpp::Any ASTBuilder::visitParameterList(StaticScriptParser::ParameterListContext *ctx) {
    SharedPtrVector<ParmVarDeclNode> params;
    if (ctx) {
        for (size_t i = 0; i < ctx->Identifier().size(); ++i) {
            String name = ctx->Identifier(i)->getText();
            SharedPtr<BuiltinTypeNode> type = visitTypeAnnotation(ctx->typeAnnotation(i));
            SharedPtr<ParmVarDeclNode> param = makeShared<ParmVarDeclNode>(name, type);
            params.push_back(param);
        }
    }
    return params;
}

antlrcpp::Any ASTBuilder::visitFunctionBody(StaticScriptParser::FunctionBodyContext *ctx) {
    return visitCompoundStatement(ctx->compoundStatement());
}

antlrcpp::Any ASTBuilder::visitCallExpression(StaticScriptParser::CallExpressionContext *ctx) {
    String calleeName = ctx->Identifier()->getText();
    SharedPtrVector<ExprNode> args = visitArgumentList(ctx->argumentList());
    SharedPtr<CallExprNode> callExpr = makeShared<CallExprNode>(calleeName, args);
    return callExpr;
}

antlrcpp::Any ASTBuilder::visitArgumentList(StaticScriptParser::ArgumentListContext *ctx) {
    return visitExpressionList(ctx->expressionList());
}

antlrcpp::Any ASTBuilder::visitCompoundStatement(StaticScriptParser::CompoundStatementContext *ctx) {
    SharedPtrVector<StmtNode> childStmts = visitStatements(ctx->statements());
    SharedPtr<CompoundStmtNode> compoundStmt = makeShared<CompoundStmtNode>(childStmts);
    return compoundStmt;
}

antlrcpp::Any ASTBuilder::visitExpressionStatement(StaticScriptParser::ExpressionStatementContext *ctx) {
    SharedPtr<ExprNode> expr = visitExpression(ctx->expression());
    SharedPtr<ExprStmtNode> exprStmt = makeShared<ExprStmtNode>(expr);
    return exprStmt;
}

antlrcpp::Any ASTBuilder::visitExpression(StaticScriptParser::ExpressionContext *ctx) {
    if (ctx) {
        SharedPtr<ExprNode> expr;
        if (ctx->literal()) {
            SharedPtr<LiteralExprNode> literalExpr = visitLiteral(ctx->literal());
            expr = literalExpr;
        } else if (ctx->OpenParen()) {
            expr = visitExpression(ctx->expression(0));
        } else if (ctx->Identifier()) {
            expr = makeShared<IdentifierExprNode>(ctx->Identifier()->getText());
        } else if (ctx->callExpression()) {
            SharedPtr<CallExprNode> callExpr = visitCallExpression(ctx->callExpression());
            expr = callExpr;
        } else if (ctx->uop) {
            SharedPtr<ExprNode> subExpr = visitExpression(ctx->expression(0));
            expr = makeShared<UnaryOperatorExprNode>(ctx->uop->getType(), subExpr);
        } else {
            SharedPtr<ExprNode> lhs = visitExpression(ctx->expression(0));
            SharedPtr<ExprNode> rhs = visitExpression(ctx->expression(1));
            expr = makeShared<BinaryOperatorExprNode>(ctx->bop->getType(), lhs, rhs);
        }
        return expr;
    }
    return nullptr;
}

antlrcpp::Any ASTBuilder::visitLiteral(StaticScriptParser::LiteralContext *ctx) {
    SharedPtr<LiteralExprNode> literalExpr;
    if (ctx->BooleanLiteral()) {
        bool literal = ctx->BooleanLiteral()->getText() == "true";
        literalExpr = makeShared<BooleanLiteralExprNode>(TypeKind::Boolean, literal);
    } else if (ctx->IntegerLiteral()) {
        int literal = std::stoi(ctx->IntegerLiteral()->getText());
        literalExpr = makeShared<IntegerLiteralExprNode>(TypeKind::Integer, literal);
    } else {
        String literal = ctx->StringLiteral()->getText();
        literal = literal.substr(1, literal.size() - 2);
        literalExpr = makeShared<StringLiteralExprNode>(TypeKind::String, literal);
    }
    return literalExpr;
}

antlrcpp::Any ASTBuilder::visitSelectionStatement(StaticScriptParser::SelectionStatementContext *ctx) {
    if (ctx->ifStatement()) {
        return visitIfStatement(ctx->ifStatement());
    }
    return nullptr;
}

antlrcpp::Any ASTBuilder::visitIfStatement(StaticScriptParser::IfStatementContext *ctx) {
    SharedPtr<ExprNode> condition = visitExpression(ctx->expression());
    SharedPtr<StmtNode> thenBody = visitStatement(ctx->statement(0));
    SharedPtr<StmtNode> elseBody;
    if (ctx->Else()) {
        elseBody = visitStatement(ctx->statement(1));
    }
    SharedPtr<IfStmtNode> ifStmt = makeShared<IfStmtNode>(condition, thenBody, elseBody);
    return ifStmt;
}

antlrcpp::Any ASTBuilder::visitIterationStatement(StaticScriptParser::IterationStatementContext *ctx) {
    if (ctx->whileStatement()) {
        return visitWhileStatement(ctx->whileStatement());
    } else if (ctx->forStatement()) {
        return visitForStatement(ctx->forStatement());
    }
    return nullptr;
}

antlrcpp::Any ASTBuilder::visitWhileStatement(StaticScriptParser::WhileStatementContext *ctx) {
    SharedPtr<ExprNode> condition = visitExpression(ctx->expression());
    SharedPtr<StmtNode> body = visitStatement(ctx->statement());
    SharedPtr<WhileStmtNode> whileStmt = makeShared<WhileStmtNode>(condition, body);
    return whileStmt;
}

antlrcpp::Any ASTBuilder::visitForStatement(StaticScriptParser::ForStatementContext *ctx) {
    antlrcpp::Any forInitAny = visitForInit(ctx->forInit());
    SharedPtr<VarDeclStmtNode> forInitVarDecls;
    SharedPtrVector<ExprNode> forInitExprList;
    if (forInitAny.isNotNull()) {
        if (forInitAny.is<SharedPtr<VarDeclStmtNode>>()) {
            forInitVarDecls = forInitAny.as<SharedPtr<VarDeclStmtNode>>();
        } else if (forInitAny.is<SharedPtrVector<ExprNode>>()) {
            forInitExprList = forInitAny.as<SharedPtrVector<ExprNode>>();
        }
    }
    SharedPtr<ExprNode> forCondition = visitExpression(ctx->forCondition);
    SharedPtrVector<ExprNode> forUpdate = visitExpressionList(ctx->forUpdate);
    SharedPtr<StmtNode> body = visitStatement(ctx->statement());
    SharedPtr<ForStmtNode> forStmt = makeShared<ForStmtNode>(forInitVarDecls, forInitExprList, forCondition, forUpdate, body);
    return forStmt;
}

antlrcpp::Any ASTBuilder::visitForInit(StaticScriptParser::ForInitContext *ctx) {
    if (ctx) {
        if (ctx->variableDeclarators()) {
            return visitVariableDeclarators(ctx->variableDeclarators());
        } else if (ctx->expressionList()) {
            return visitExpressionList(ctx->expressionList());
        }
    }
    return nullptr;
}

antlrcpp::Any ASTBuilder::visitExpressionList(StaticScriptParser::ExpressionListContext *ctx) {
    SharedPtrVector<ExprNode> exprList;
    for (auto exprCtx : ctx->expression()) {
        SharedPtr<ExprNode> expr = visitExpression(exprCtx);
        exprList.push_back(expr);
    }
    return exprList;
}

antlrcpp::Any ASTBuilder::visitJumpStatement(StaticScriptParser::JumpStatementContext *ctx) {
    if (ctx->continueStatement()) {
        return visitContinueStatement(ctx->continueStatement());
    } else if (ctx->breakStatement()) {
        return visitBreakStatement(ctx->breakStatement());
    } else if (ctx->returnStatement()) {
        return visitReturnStatement(ctx->returnStatement());
    }
    return nullptr;
}

antlrcpp::Any ASTBuilder::visitContinueStatement(StaticScriptParser::ContinueStatementContext *ctx) {
    SharedPtr<ContinueStmtNode> continueStmt = makeShared<ContinueStmtNode>();
    return continueStmt;
}

antlrcpp::Any ASTBuilder::visitBreakStatement(StaticScriptParser::BreakStatementContext *ctx) {
    SharedPtr<BreakStmtNode> breakStmt = makeShared<BreakStmtNode>();
    return breakStmt;
}

antlrcpp::Any ASTBuilder::visitReturnStatement(StaticScriptParser::ReturnStatementContext *ctx) {
    SharedPtr<ExprNode> argument = visitExpression(ctx->expression());
    SharedPtr<ReturnStmtNode> returnStmt = makeShared<ReturnStmtNode>(argument);
    return returnStmt;
}
