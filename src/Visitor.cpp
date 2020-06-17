#include "Visitor.h"

antlrcpp::Any Visitor::visitModule(StaticScriptParser::ModuleContext *ctx) {
    return StaticScriptParserBaseVisitor::visitModule(ctx);
}

antlrcpp::Any Visitor::visitStatements(StaticScriptParser::StatementsContext *ctx) {
    return StaticScriptParserBaseVisitor::visitStatements(ctx);
}

antlrcpp::Any Visitor::visitStatement(StaticScriptParser::StatementContext *ctx) {
    return StaticScriptParserBaseVisitor::visitStatement(ctx);
}

antlrcpp::Any Visitor::visitDeclaration(StaticScriptParser::DeclarationContext *ctx) {
    return StaticScriptParserBaseVisitor::visitDeclaration(ctx);
}

antlrcpp::Any Visitor::visitVariableDeclaration(StaticScriptParser::VariableDeclarationContext *ctx) {
    return StaticScriptParserBaseVisitor::visitVariableDeclaration(ctx);
}

antlrcpp::Any Visitor::visitVariableDeclarators(StaticScriptParser::VariableDeclaratorsContext *ctx) {
    return StaticScriptParserBaseVisitor::visitVariableDeclarators(ctx);
}

antlrcpp::Any Visitor::visitVariableModifier(StaticScriptParser::VariableModifierContext *ctx) {
    return StaticScriptParserBaseVisitor::visitVariableModifier(ctx);
}

antlrcpp::Any Visitor::visitVariableDeclarator(StaticScriptParser::VariableDeclaratorContext *ctx) {
    return StaticScriptParserBaseVisitor::visitVariableDeclarator(ctx);
}

antlrcpp::Any Visitor::visitTypeAnnotation(StaticScriptParser::TypeAnnotationContext *ctx) {
    return StaticScriptParserBaseVisitor::visitTypeAnnotation(ctx);
}

antlrcpp::Any Visitor::visitVariableInitializer(StaticScriptParser::VariableInitializerContext *ctx) {
    return StaticScriptParserBaseVisitor::visitVariableInitializer(ctx);
}

antlrcpp::Any Visitor::visitPredefinedType(StaticScriptParser::PredefinedTypeContext *ctx) {
    return StaticScriptParserBaseVisitor::visitPredefinedType(ctx);
}

antlrcpp::Any Visitor::visitFunctionDeclaration(StaticScriptParser::FunctionDeclarationContext *ctx) {
    return StaticScriptParserBaseVisitor::visitFunctionDeclaration(ctx);
}

antlrcpp::Any Visitor::visitCallSignature(StaticScriptParser::CallSignatureContext *ctx) {
    return StaticScriptParserBaseVisitor::visitCallSignature(ctx);
}

antlrcpp::Any Visitor::visitParameterList(StaticScriptParser::ParameterListContext *ctx) {
    return StaticScriptParserBaseVisitor::visitParameterList(ctx);
}

antlrcpp::Any Visitor::visitFunctionBody(StaticScriptParser::FunctionBodyContext *ctx) {
    return StaticScriptParserBaseVisitor::visitFunctionBody(ctx);
}

antlrcpp::Any Visitor::visitFunctionCallExpression(StaticScriptParser::FunctionCallExpressionContext *ctx) {
    return StaticScriptParserBaseVisitor::visitFunctionCallExpression(ctx);
}

antlrcpp::Any Visitor::visitArgumentList(StaticScriptParser::ArgumentListContext *ctx) {
    return StaticScriptParserBaseVisitor::visitArgumentList(ctx);
}

antlrcpp::Any Visitor::visitCompoundStatement(StaticScriptParser::CompoundStatementContext *ctx) {
    return StaticScriptParserBaseVisitor::visitCompoundStatement(ctx);
}

antlrcpp::Any Visitor::visitExpressionStatement(StaticScriptParser::ExpressionStatementContext *ctx) {
    return StaticScriptParserBaseVisitor::visitExpressionStatement(ctx);
}

antlrcpp::Any Visitor::visitExpression(StaticScriptParser::ExpressionContext *ctx) {
    return StaticScriptParserBaseVisitor::visitExpression(ctx);
}

antlrcpp::Any Visitor::visitLiteral(StaticScriptParser::LiteralContext *ctx) {
    return StaticScriptParserBaseVisitor::visitLiteral(ctx);
}

antlrcpp::Any Visitor::visitSelectionStatement(StaticScriptParser::SelectionStatementContext *ctx) {
    return StaticScriptParserBaseVisitor::visitSelectionStatement(ctx);
}

antlrcpp::Any Visitor::visitIfElseStatement(StaticScriptParser::IfElseStatementContext *ctx) {
    return StaticScriptParserBaseVisitor::visitIfElseStatement(ctx);
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