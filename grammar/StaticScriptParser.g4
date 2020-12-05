parser grammar StaticScriptParser;

options {
	language   = Cpp;
	tokenVocab = StaticScriptLexer;
}

module
    : statements? EOF
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

variableInitializer
    : Assign expression
    ;

typeAnnotation
    : Colon type
    ;

type
    : arrayType
    | basicType
    ;

arrayType
    : basicType (OpenBracket CloseBracket)+
    ;

basicType
    : Boolean
    | Integer
    | Number
    | String
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

compoundStatement
    : OpenBrace statements? CloseBrace
    ;

expressionStatement
    : expression SemiColon
    ;

expression
    : base=expression (OpenBracket index+=expression CloseBracket)+                                     # ArraySubscriptExpr
    | callExpression                                                                                    # CallExpr
    | expression uop=(PlusPlus | MinusMinus)                                                            # PostfixUnaryExpr
    | <assoc=right> uop=(Not | BitNot | Plus | Minus | PlusPlus | MinusMinus) expression                # PrefixUnaryExpr
    | expression bop=(Multiply | Divide | Modulus) expression                                           # BinaryExpr
    | expression bop=(Plus | Minus) expression                                                          # BinaryExpr
    | expression bop=(ShiftLeft | ArithmeticShiftRight | LogicalShiftRight) expression                                                # BinaryExpr
    | expression bop=(LessThan | GreaterThan | LessThanEquals | GreaterThanEquals) expression           # BinaryExpr
    | expression bop=(Equals | NotEquals) expression                                                    # BinaryExpr
    | expression bop=BitAnd expression                                                                  # BinaryExpr
    | expression bop=BitXOr expression                                                                  # BinaryExpr
    | expression bop=BitOr expression                                                                   # BinaryExpr
    | expression bop=And expression                                                                     # BinaryExpr
    | expression bop=Or expression                                                                      # BinaryExpr
    | <assoc=right> expression bop=(Assign | PlusAssign | MinusAssign | MultiplyAssign | DivideAssign | ModulusAssign | ShiftLeftAssign | ArithmeticShiftRightAssign | LogicalShiftRightAssign | BitAndAssign | BitXorAssign | BitOrAssign | AndAssign | OrAssign) expression # BinaryExpr
    | Identifier                                                                                        # IdentifierExpr
    | literal                                                                                           # LiteralExpr
    | OpenParen expression CloseParen                                                                   # ParenExpr
    ;

callExpression
    : Identifier OpenParen argumentList? CloseParen
    ;

argumentList
    : expressionList
    ;

literal
    : BooleanLiteral
    | DecimalIntegerLiteral
    | HexIntegerLiteral
    | OctalIntegerLiteral
    | BinaryIntegerLiteral
    | FloatLiteral
    | StringLiteral
    | arrayLiteral
    ;

arrayLiteral
    : OpenBracket expressionList? CloseBracket
    ;

expressionList
    : expression (Comma expression)*
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