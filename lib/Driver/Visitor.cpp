#include "Driver/Visitor.h"
#include "StaticScriptLexer.h"

Visitor::Visitor(String filename) : filename(std::move(filename)) {}

antlrcpp::Any Visitor::visitModule(StaticScriptParser::ModuleContext *ctx) {
    StaticScriptParser::StatementsContext *stmtsCtx = ctx->statements();
    if (stmtsCtx) {
        const SharedPtrVector<Stmt> &childStmts = visitStatements(stmtsCtx);
        return makeShared<Module>(filename, childStmts);
    }
    return makeShared<Module>(filename);
}

antlrcpp::Any Visitor::visitStatements(StaticScriptParser::StatementsContext *ctx) {
    SharedPtrVector<Stmt> stmts;
    for (auto &stmtCtx: ctx->statement()) {
        antlrcpp::Any stmtAny = visitStatement(stmtCtx);
        if (stmtAny.isNotNull()) {
            SharedPtr<Stmt> stmt;
            if (stmtAny.is<SharedPtr<VarDeclStmt>>()) {
                stmt = stmtAny.as<SharedPtr<VarDeclStmt>>();
            }
            stmts.push_back(stmt);
        }
    }
    return stmts;
}

antlrcpp::Any Visitor::visitStatement(StaticScriptParser::StatementContext *ctx) {
    if (ctx->emptyStatement()) {
        return visitEmptyStatement(ctx->emptyStatement());
    } else if (ctx->declarationStatement()) {
        return visitDeclarationStatement(ctx->declarationStatement());
    } else if (ctx->compoundStatement()) {
        return visitCompoundStatement(ctx->compoundStatement());
    } else if (ctx->expressionStatement()) {
        return visitExpressionStatement(ctx->expressionStatement());
    } else if (ctx->selectionStatement()) {
        return visitSelectionStatement(ctx->selectionStatement());
    } else if (ctx->iterationStatement()) {
        return visitIterationStatement(ctx->iterationStatement());
    } else if (ctx->jumpStatement()) {
        return visitJumpStatement(ctx->jumpStatement());
    }
    return nullptr;
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
    // TODO: 此处有转换风险
    varDecl->type = visitTypeAnnotation(ctx->typeAnnotation());
    varDecl->name = ctx->Identifier()->getText();
    varDecl->initVal = visitVariableInitializer(ctx->variableInitializer());
    return varDecl;
}

antlrcpp::Any Visitor::visitTypeAnnotation(StaticScriptParser::TypeAnnotationContext *ctx) {
    if (ctx != nullptr) {
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
    return StaticScriptParserBaseVisitor::visitFunctionDeclaration(ctx);
}

antlrcpp::Any Visitor::visitParameterList(StaticScriptParser::ParameterListContext *ctx) {
    return StaticScriptParserBaseVisitor::visitParameterList(ctx);
}

antlrcpp::Any Visitor::visitFunctionBody(StaticScriptParser::FunctionBodyContext *ctx) {
    return StaticScriptParserBaseVisitor::visitFunctionBody(ctx);
}

antlrcpp::Any Visitor::visitCallExpression(StaticScriptParser::CallExpressionContext *ctx) {
    return StaticScriptParserBaseVisitor::visitCallExpression(ctx);
}

antlrcpp::Any Visitor::visitArgumentList(StaticScriptParser::ArgumentListContext *ctx) {
    return StaticScriptParserBaseVisitor::visitArgumentList(ctx);
}

antlrcpp::Any Visitor::visitCompoundStatement(StaticScriptParser::CompoundStatementContext *ctx) {
    return StaticScriptParserBaseVisitor::visitCompoundStatement(ctx);
}

antlrcpp::Any Visitor::visitExpressionStatement(StaticScriptParser::ExpressionStatementContext *ctx) {
    return visitExpression(ctx->expression());
}

antlrcpp::Any Visitor::visitExpression(StaticScriptParser::ExpressionContext *ctx) {
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
    return StaticScriptParserBaseVisitor::visitSelectionStatement(ctx);
}

antlrcpp::Any Visitor::visitIfStatement(StaticScriptParser::IfStatementContext *ctx) {
    return StaticScriptParserBaseVisitor::visitIfStatement(ctx);
}

antlrcpp::Any Visitor::visitIterationStatement(StaticScriptParser::IterationStatementContext *ctx) {
    return StaticScriptParserBaseVisitor::visitIterationStatement(ctx);
}

antlrcpp::Any Visitor::visitForInitExpr(StaticScriptParser::ForInitExprContext *ctx) {
    return StaticScriptParserBaseVisitor::visitForInitExpr(ctx);
}

antlrcpp::Any Visitor::visitForCondExpr(StaticScriptParser::ForCondExprContext *ctx) {
    return StaticScriptParserBaseVisitor::visitForCondExpr(ctx);
}

antlrcpp::Any Visitor::visitForLoopExpr(StaticScriptParser::ForLoopExprContext *ctx) {
    return StaticScriptParserBaseVisitor::visitForLoopExpr(ctx);
}

antlrcpp::Any Visitor::visitExpressionList(StaticScriptParser::ExpressionListContext *ctx) {
    return StaticScriptParserBaseVisitor::visitExpressionList(ctx);
}

antlrcpp::Any Visitor::visitJumpStatement(StaticScriptParser::JumpStatementContext *ctx) {
    return StaticScriptParserBaseVisitor::visitJumpStatement(ctx);
}

antlrcpp::Any Visitor::visitContinueStatement(StaticScriptParser::ContinueStatementContext *ctx) {
    return StaticScriptParserBaseVisitor::visitContinueStatement(ctx);
}

antlrcpp::Any Visitor::visitBreakStatement(StaticScriptParser::BreakStatementContext *ctx) {
    return StaticScriptParserBaseVisitor::visitBreakStatement(ctx);
}

antlrcpp::Any Visitor::visitReturnStatement(StaticScriptParser::ReturnStatementContext *ctx) {
    return StaticScriptParserBaseVisitor::visitReturnStatement(ctx);
}
