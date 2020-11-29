#include "AST/ASTBuilder.h"

ASTBuilder::ASTBuilder(String filename) : filename(std::move(filename)) {}

antlrcpp::Any ASTBuilder::visitModule(StaticScriptParser::ModuleContext *ctx) {
    SharedPtrVector<StmtNode> childStmts;
    if (ctx->statements()) {
        antlrcpp::Any stmtsAny = visitStatements(ctx->statements());
        childStmts = stmtsAny.as<SharedPtrVector<StmtNode>>();
    }
    SharedPtr<ModuleNode> module = makeShared<ModuleNode>(filename, childStmts);
    module->bindChildrenInversely();
    return module;
}

antlrcpp::Any ASTBuilder::visitStatements(StaticScriptParser::StatementsContext *ctx) {
    SharedPtrVector<StmtNode> stmts;
    for (auto &stmtCtx: ctx->statement()) {
        const SharedPtr<StmtNode> &stmt = visitStatement(stmtCtx);
        stmts.push_back(stmt);
    }
    return stmts;
}

antlrcpp::Any ASTBuilder::visitStatement(StaticScriptParser::StatementContext *ctx) {
    antlrcpp::Any stmtAny;
    SharedPtr<StmtNode> stmt;
    if (ctx->declarationStatement()) {
        stmtAny = visitDeclarationStatement(ctx->declarationStatement());
        if (stmtAny.is<SharedPtr<VarDeclStmtNode>>()) {
            stmt = stmtAny.as<SharedPtr<VarDeclStmtNode>>();
        } else if (stmtAny.is<SharedPtr<FunctionDeclStmtNode>>()) {
            stmt = stmtAny.as<SharedPtr<FunctionDeclStmtNode>>();
        }
    } else if (ctx->compoundStatement()) {
        stmtAny = visitCompoundStatement(ctx->compoundStatement());
        stmt = stmtAny.as<SharedPtr<CompoundStmtNode>>();
    } else if (ctx->expressionStatement()) {
        stmtAny = visitExpressionStatement(ctx->expressionStatement());
        stmt = stmtAny.as<SharedPtr<ExprStmtNode>>();
    } else if (ctx->selectionStatement()) {
        stmtAny = visitSelectionStatement(ctx->selectionStatement());
        stmt = stmtAny.as<SharedPtr<IfStmtNode>>();
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
    }
    return visitFunctionDeclaration(ctx->functionDeclaration());
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
    varDeclStmt->bindChildrenInversely();
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
    varDecl->name = ctx->Identifier()->getText();
    if (ctx->typeAnnotation()) {
        varDecl->type = visitTypeAnnotation(ctx->typeAnnotation());
    }
    if (ctx->variableInitializer()) {
        varDecl->initVal = visitVariableInitializer(ctx->variableInitializer());
    }
    varDecl->bindChildrenInversely();
    return varDecl;
}

antlrcpp::Any ASTBuilder::visitVariableInitializer(StaticScriptParser::VariableInitializerContext *ctx) {
    return visitExpression(ctx->expression());
}

antlrcpp::Any ASTBuilder::visitTypeAnnotation(StaticScriptParser::TypeAnnotationContext *ctx) {
    return visitType(ctx->type());
}

antlrcpp::Any ASTBuilder::visitType(StaticScriptParser::TypeContext *ctx) {
    SharedPtr<Type> type;
    if (ctx->arrayType()) {
        type = visitArrayType(ctx->arrayType()).as<SharedPtr<ArrayType>>();
    } else {
        type = visitAtomicType(ctx->atomicType()).as<SharedPtr<AtomicType>>();
    }
    return type;
}

antlrcpp::Any ASTBuilder::visitArrayType(StaticScriptParser::ArrayTypeContext *ctx) {
    const SharedPtr<AtomicType> &atomicType = visitAtomicType(ctx->atomicType());
    size_t depth = ctx->OpenBracket().size();
    return ArrayType::createNDArrayType(atomicType, depth);
}

antlrcpp::Any ASTBuilder::visitAtomicType(StaticScriptParser::AtomicTypeContext *ctx) {
    if (ctx->Boolean()) {
        return AtomicType::BOOLEAN_TYPE;
    } else if (ctx->Number()) {
        return AtomicType::INTEGER_TYPE;
    }
    return AtomicType::STRING_TYPE;
}

antlrcpp::Any ASTBuilder::visitFunctionDeclaration(StaticScriptParser::FunctionDeclarationContext *ctx) {
    String name = ctx->Identifier()->getText();
    SharedPtrVector<ParmVarDeclNode> params;
    SharedPtr<Type> returnType;
    if (ctx->parameterList()) {
        params = visitParameterList(ctx->parameterList()).as<SharedPtrVector<ParmVarDeclNode>>();
    }
    if (ctx->typeAnnotation()) {
        returnType = visitTypeAnnotation(ctx->typeAnnotation());
    }
    SharedPtr<CompoundStmtNode> body = visitFunctionBody(ctx->functionBody());
    SharedPtr<FunctionDeclNode> functionDecl = makeShared<FunctionDeclNode>(name, params, returnType, body);
    SharedPtr<FunctionDeclStmtNode> functionDeclStmt = makeShared<FunctionDeclStmtNode>(functionDecl);
    functionDecl->bindChildrenInversely();
    functionDeclStmt->bindChildrenInversely();
    return functionDeclStmt;
}

antlrcpp::Any ASTBuilder::visitParameterList(StaticScriptParser::ParameterListContext *ctx) {
    SharedPtrVector<ParmVarDeclNode> params;
    for (size_t i = 0; i < ctx->Identifier().size(); ++i) {
        String name = ctx->Identifier(i)->getText();
        SharedPtr<Type> type = visitTypeAnnotation(ctx->typeAnnotation(i));
        SharedPtr<ParmVarDeclNode> param = makeShared<ParmVarDeclNode>(name, type);
        param->bindChildrenInversely();
        params.push_back(param);
    }
    return params;
}

antlrcpp::Any ASTBuilder::visitFunctionBody(StaticScriptParser::FunctionBodyContext *ctx) {
    return visitCompoundStatement(ctx->compoundStatement());
}

antlrcpp::Any ASTBuilder::visitCompoundStatement(StaticScriptParser::CompoundStatementContext *ctx) {
    SharedPtrVector<StmtNode> childStmts;
    if (ctx->statements()) {
        antlrcpp::Any stmtsAny = visitStatements(ctx->statements());
        childStmts = stmtsAny.as<SharedPtrVector<StmtNode>>();
    }
    SharedPtr<CompoundStmtNode> compoundStmt = makeShared<CompoundStmtNode>(childStmts);
    compoundStmt->bindChildrenInversely();
    return compoundStmt;
}

antlrcpp::Any ASTBuilder::visitExpressionStatement(StaticScriptParser::ExpressionStatementContext *ctx) {
    SharedPtr<ExprNode> expr = visitExpression(ctx->expression());
    SharedPtr<ExprStmtNode> exprStmt = makeShared<ExprStmtNode>(expr);
    exprStmt->bindChildrenInversely();
    return exprStmt;
}

antlrcpp::Any ASTBuilder::visitExpression(StaticScriptParser::ExpressionContext *ctx) {
    SharedPtr<ExprNode> expr;
    antlrcpp::Any exprAny;
    if (auto arraySubscriptExprCtx = dynamic_cast<StaticScriptParser::ArraySubscriptExprContext *>(ctx)) {
        exprAny = visitArraySubscriptExpr(arraySubscriptExprCtx);
        expr = exprAny.as<SharedPtr<ArraySubscriptExprNode>>();
    } else if (auto callExprCtx = dynamic_cast<StaticScriptParser::CallExprContext *>(ctx)) {
        exprAny = visitCallExpr(callExprCtx);
        expr = exprAny.as<SharedPtr<CallExprNode>>();
    } else if (auto postfixUnaryExprCtx = dynamic_cast<StaticScriptParser::PostfixUnaryExprContext *>(ctx)) {
        exprAny = visitPostfixUnaryExpr(postfixUnaryExprCtx);
        expr = exprAny.as<SharedPtr<UnaryOperatorExprNode>>();
    } else if (auto prefixUnaryExprCtx = dynamic_cast<StaticScriptParser::PrefixUnaryExprContext *>(ctx)) {
        exprAny = visitPrefixUnaryExpr(prefixUnaryExprCtx);
        expr = exprAny.as<SharedPtr<UnaryOperatorExprNode>>();
    } else if (auto binaryExprCtx = dynamic_cast<StaticScriptParser::BinaryExprContext *>(ctx)) {
        exprAny = visitBinaryExpr(binaryExprCtx);
        expr = exprAny.as<SharedPtr<BinaryOperatorExprNode>>();
    } else if (auto idExprCtx = dynamic_cast<StaticScriptParser::IdentifierExprContext *>(ctx)) {
        exprAny = visitIdentifierExpr(idExprCtx);
        expr = exprAny.as<SharedPtr<IdentifierExprNode>>();
    } else if (auto literalExprCtx = dynamic_cast<StaticScriptParser::LiteralExprContext *>(ctx)) {
        exprAny = visitLiteralExpr(literalExprCtx);
        expr = exprAny.as<SharedPtr<LiteralExprNode>>();
    } else if (auto parenExprCtx = dynamic_cast<StaticScriptParser::ParenExprContext *>(ctx)) {
        exprAny = visitParenExpr(parenExprCtx);
        expr = exprAny.as<SharedPtr<ExprNode>>();
    }
    expr->bindChildrenInversely();
    return expr;
}

antlrcpp::Any ASTBuilder::visitArraySubscriptExpr(StaticScriptParser::ArraySubscriptExprContext *ctx) {
    SharedPtr<ExprNode> baseExpr = visitExpression(ctx->base);
    SharedPtrVector<ExprNode> indexExprs(ctx->index.size());
    for (size_t i = 0; i < ctx->index.size(); ++i) {
        indexExprs[i] = visitExpression(ctx->index[i]).as<SharedPtr<ExprNode>>();
    }
    return makeShared<ArraySubscriptExprNode>(baseExpr, indexExprs);
}

antlrcpp::Any ASTBuilder::visitCallExpr(StaticScriptParser::CallExprContext *ctx) {
    return visitCallExpression(ctx->callExpression());
}

antlrcpp::Any ASTBuilder::visitPostfixUnaryExpr(StaticScriptParser::PostfixUnaryExprContext *ctx) {
    SharedPtr<ExprNode> subExpr = visitExpression(ctx->expression());
    SharedPtr<UnaryOperatorExprNode> expr = makeShared<UnaryOperatorExprNode>(ctx->uop->getType(), subExpr);
    expr->isPostfix = true;
    return expr;
}

antlrcpp::Any ASTBuilder::visitPrefixUnaryExpr(StaticScriptParser::PrefixUnaryExprContext *ctx) {
    SharedPtr<ExprNode> subExpr = visitExpression(ctx->expression());
    return makeShared<UnaryOperatorExprNode>(ctx->uop->getType(), subExpr);
}

antlrcpp::Any ASTBuilder::visitBinaryExpr(StaticScriptParser::BinaryExprContext *ctx) {
    SharedPtr<ExprNode> lhs = visitExpression(ctx->expression(0));
    SharedPtr<ExprNode> rhs = visitExpression(ctx->expression(1));
    return makeShared<BinaryOperatorExprNode>(ctx->bop->getType(), lhs, rhs);
}

antlrcpp::Any ASTBuilder::visitIdentifierExpr(StaticScriptParser::IdentifierExprContext *ctx) {
    return makeShared<IdentifierExprNode>(ctx->Identifier()->getText());
}

antlrcpp::Any ASTBuilder::visitLiteralExpr(StaticScriptParser::LiteralExprContext *ctx) {
    return visitLiteral(ctx->literal());
}

antlrcpp::Any ASTBuilder::visitParenExpr(StaticScriptParser::ParenExprContext *ctx) {
    return visitExpression(ctx->expression());
}

antlrcpp::Any ASTBuilder::visitCallExpression(StaticScriptParser::CallExpressionContext *ctx) {
    String calleeName = ctx->Identifier()->getText();
    SharedPtrVector<ExprNode> args;
    if (ctx->argumentList()) {
        args = visitArgumentList(ctx->argumentList()).as<SharedPtrVector<ExprNode>>();
    }
    SharedPtr<CallExprNode> callExpr = makeShared<CallExprNode>(calleeName, args);
    callExpr->bindChildrenInversely();
    return callExpr;
}

antlrcpp::Any ASTBuilder::visitArgumentList(StaticScriptParser::ArgumentListContext *ctx) {
    return visitExpressionList(ctx->expressionList());
}

antlrcpp::Any ASTBuilder::visitLiteral(StaticScriptParser::LiteralContext *ctx) {
    SharedPtr<LiteralExprNode> literalExpr;
    if (ctx->BooleanLiteral()) {
        bool literal = ctx->BooleanLiteral()->getText() == "true";
        literalExpr = makeShared<BooleanLiteralExprNode>(literal);
    } else if (ctx->IntegerLiteral()) {
        int literal = std::stoi(ctx->IntegerLiteral()->getText());
        literalExpr = makeShared<IntegerLiteralExprNode>(literal);
    } else if (ctx->StringLiteral()) {
        String literal = ctx->StringLiteral()->getText();
        literal = literal.substr(1, literal.size() - 2);
        literalExpr = makeShared<StringLiteralExprNode>(literal);
    } else if (ctx->arrayLiteral()) {
        literalExpr = visitArrayLiteral(ctx->arrayLiteral()).as<SharedPtr<ArrayLiteralExprNode>>();
    }
    return literalExpr;
}

antlrcpp::Any ASTBuilder::visitArrayLiteral(StaticScriptParser::ArrayLiteralContext *ctx) {
    SharedPtr<ArrayLiteralExprNode> arrayLiteralExpr = makeShared<ArrayLiteralExprNode>();
    if (ctx->expressionList()) {
        arrayLiteralExpr->elements = visitExpressionList(ctx->expressionList()).as<SharedPtrVector<ExprNode>>();
    }
    return arrayLiteralExpr;
}

antlrcpp::Any ASTBuilder::visitSelectionStatement(StaticScriptParser::SelectionStatementContext *ctx) {
    return visitIfStatement(ctx->ifStatement());
}

antlrcpp::Any ASTBuilder::visitIfStatement(StaticScriptParser::IfStatementContext *ctx) {
    SharedPtr<ExprNode> condition = visitExpression(ctx->ifCondition);
    SharedPtr<StmtNode> thenBody = visitStatement(ctx->thenBody);
    SharedPtr<StmtNode> elseBody;
    if (ctx->Else()) {
        elseBody = visitStatement(ctx->elseBody);
    }
    SharedPtr<IfStmtNode> ifStmt = makeShared<IfStmtNode>(condition, thenBody, elseBody);
    ifStmt->bindChildrenInversely();
    return ifStmt;
}

antlrcpp::Any ASTBuilder::visitIterationStatement(StaticScriptParser::IterationStatementContext *ctx) {
    if (ctx->whileStatement()) {
        return visitWhileStatement(ctx->whileStatement());
    }
    return visitForStatement(ctx->forStatement());
}

antlrcpp::Any ASTBuilder::visitWhileStatement(StaticScriptParser::WhileStatementContext *ctx) {
    SharedPtr<ExprNode> condition = visitExpression(ctx->whileCondition);
    SharedPtr<StmtNode> body = visitStatement(ctx->whileBody);
    SharedPtr<WhileStmtNode> whileStmt = makeShared<WhileStmtNode>(condition, body);
    whileStmt->bindChildrenInversely();
    return whileStmt;
}

antlrcpp::Any ASTBuilder::visitForStatement(StaticScriptParser::ForStatementContext *ctx) {
    SharedPtr<VarDeclStmtNode> forInitVarDecls;
    SharedPtrVector<ExprNode> forInitExprList;
    SharedPtr<ExprNode> forCondition;
    SharedPtrVector<ExprNode> forUpdate;
    if (ctx->forInit()) {
        antlrcpp::Any forInitAny = visitForInit(ctx->forInit());
        if (forInitAny.is<SharedPtr<VarDeclStmtNode>>()) {
            forInitVarDecls = forInitAny.as<SharedPtr<VarDeclStmtNode>>();
        } else if (forInitAny.is<SharedPtrVector<ExprNode>>()) {
            forInitExprList = forInitAny.as<SharedPtrVector<ExprNode>>();
        }
    }
    if (ctx->forCondition) {
        forCondition = visitExpression(ctx->forCondition);
    }
    if (ctx->forUpdate) {
        antlrcpp::Any forUpdateAny = visitExpressionList(ctx->forUpdate);
        forUpdate = forUpdateAny.as<SharedPtrVector<ExprNode>>();
    }
    SharedPtr<StmtNode> body = visitStatement(ctx->forBody);
    SharedPtr<ForStmtNode> forStmt = makeShared<ForStmtNode>(forInitVarDecls, forInitExprList, forCondition, forUpdate, body);
    forStmt->bindChildrenInversely();
    return forStmt;
}

antlrcpp::Any ASTBuilder::visitForInit(StaticScriptParser::ForInitContext *ctx) {
    if (ctx->variableDeclarators()) {
        return visitVariableDeclarators(ctx->variableDeclarators());
    }
    return visitExpressionList(ctx->expressionList());
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
    }
    return visitReturnStatement(ctx->returnStatement());
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
    SharedPtr<ExprNode> returnExpr;
    if (ctx->returnExpr) {
        returnExpr = visitExpression(ctx->expression());
    }
    SharedPtr<ReturnStmtNode> returnStmt = makeShared<ReturnStmtNode>(returnExpr);

    returnStmt->bindChildrenInversely();

    return returnStmt;
}
