parser grammar StaticScriptParser;

options {
	language   = Cpp;
	tokenVocab = StaticScriptLexer;
}

module
    : statements?
    ;

statements
    : statement+
    ;

statement
    : emptyStatement
    | declarationStatement
    | compoundStatement
    | expressionStatement
    | selectionStatement
    | iterationStatement
    | jumpStatement
    ;

emptyStatement
    : SemiColon
    ;

declarationStatement
    : declaration
    ;

declaration
    : variableDeclaration
    | functionDeclaration
    ;

variableDeclaration
    : variableDeclarators SemiColon
    ;

variableDeclarators
    : variableModifier variableDeclarator (Comma variableDeclarator)*
    ;

variableModifier
    : Let
    | Const
    ;

variableDeclarator
    : Identifier typeAnnotation? variableInitializer?
    ;

typeAnnotation
    : Colon builtinType
    ;

builtinType
    : Boolean
    | Number
    | String
    ;

variableInitializer
    : Assign expression
    ;

functionDeclaration
    : Function Identifier OpenParen parameterList? CloseParen typeAnnotation? functionBody
    ;

parameterList
    : Identifier typeAnnotation (Comma Identifier typeAnnotation)*
    ;

functionBody
    : compoundStatement
    ;

callExpression
    : Identifier OpenParen argumentList? CloseParen
    ;

argumentList
    : expressionList
    ;

compoundStatement
    : OpenBrace statements? CloseBrace
    ;

expressionStatement
    : expression SemiColon
    ;

expression
    : literal
    | OpenParen expression CloseParen
    | Identifier
    | callExpression
    | uop=Minus expression
    | expression bop=(Multiply | Divide) expression
    | expression bop=(Plus | Minus) expression
    | expression bop=(LessThan | GreaterThan | LessThanEquals | GreaterThanEquals) expression
    | expression bop=(Equals | NotEquals) expression
    | <assoc=right> expression bop=Assign expression
    ;

literal
    : BooleanLiteral
    | IntegerLiteral
    | StringLiteral
    ;

selectionStatement
    : ifStatement
    ;

ifStatement
    : If OpenParen ifCondition=expression CloseParen thenBody=statement (Else elseBody=statement)?
    ;

iterationStatement
    : whileStatement
    | forStatement
    ;

whileStatement
    : While OpenParen whileCondition=expression CloseParen whileBody=statement
    ;

forStatement
    : For OpenParen forInit? SemiColon forCondition=expression? SemiColon forUpdate=expressionList? CloseParen forBody=statement
    ;

forInit
    : variableDeclarators
    | expressionList
    ;

expressionList
    : expression (Comma expression)*
    ;

jumpStatement
    : continueStatement
    | breakStatement
    | returnStatement
    ;

continueStatement
    : Continue SemiColon
    ;

breakStatement
    : Break SemiColon
    ;

returnStatement
    : Return returnExpr=expression? SemiColon
    ;