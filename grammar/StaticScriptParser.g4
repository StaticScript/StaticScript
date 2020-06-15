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
    : declaration
    | compoundStatement
    | expressionStatement
    | selectionStatement
    | iterationStatement
    | jumpStatement
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
    : Colon predefinedType
    ;

variableInitializer
    : Assign expression
    ;

predefinedType
    : Void
    | Boolean
    | Number
    | String
    ;

functionDeclaration
    : Function Identifier callSignature functionBody
    ;

callSignature
    : OpenParen parameterList? CloseParen typeAnnotation?
    ;

parameterList
    : Identifier typeAnnotation? (Comma Identifier typeAnnotation?)*
    ;

functionBody
    : compoundStatement
    ;

functionCallExpression
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
    | functionCallExpression
    | expression (Multiply | Divide) expression
    | expression (Plus | Minus) expression
    | expression (LessThan | MoreThan | LessThanEquals | GreaterThanEquals) expression
    | expression (Equals | NotEquals) expression
    | <assoc=right> expression Assign expression
    ;

literal
    : BooleanLiteral
    | DecimalIntegerLiteral
    | StringLiteral
    ;

selectionStatement
    : ifElseStatement
    ;

ifElseStatement
    : If OpenParen expression CloseParen statement (Else statement)?
    ;

iterationStatement
    : While OpenParen expression CloseParen statement
    | For OpenParen forInitExpr SemiColon forCondExpr SemiColon forLoopExpr CloseParen statement
    ;

forInitExpr
    : (expressionList | variableDeclarators)?
    ;

forCondExpr
    : expression?
    ;

forLoopExpr
    : expressionList?
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
    : Return expression? SemiColon
    ;