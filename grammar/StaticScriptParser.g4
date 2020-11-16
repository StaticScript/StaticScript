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
    : literal                                                                                           # LiteralExpr
    | Identifier                                                                                        # IdentifierExpr
    | OpenParen expression CloseParen                                                                   # ParenExpr
    | callExpression                                                                                    # CallExpr
    | expression uop=(PlusPlus | MinusMinus)                                                            # PostfixUnaryExpr
    | <assoc=right> uop=(Not | BitNot | Plus | Minus | PlusPlus | MinusMinus) expression                # PrefixUnaryExpr
    | expression bop=(Multiply | Divide | Modulus) expression                                           # BinaryExpr
    | expression bop=(Plus | Minus) expression                                                          # BinaryExpr
    | expression bop=(LeftShift | RightShift) expression                                                # BinaryExpr
    | expression bop=(LessThan | GreaterThan | LessThanEquals | GreaterThanEquals) expression           # BinaryExpr
    | expression bop=(Equals | NotEquals) expression                                                    # BinaryExpr
    | expression bop=BitAnd expression                                                                  # BinaryExpr
    | expression bop=BitXOr expression                                                                  # BinaryExpr
    | expression bop=BitOr expression                                                                   # BinaryExpr
    | expression bop=And expression                                                                     # BinaryExpr
    | expression bop=Or expression                                                                      # BinaryExpr
    | <assoc=right> expression bop=(Assign | PlusAssign | MinusAssign | MultiplyAssign | DivideAssign | ModulusAssign | LeftShiftAssign | RightShiftAssign | BitAndAssign | BitXorAssign | BitOrAssign | AndAssign | OrAssign) expression # BinaryExpr
    ;

callExpression
    : Identifier OpenParen argumentList? CloseParen
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