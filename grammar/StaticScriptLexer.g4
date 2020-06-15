lexer grammar StaticScriptLexer;

options {
	language   = Cpp;
}

// 关键字
Void:                   'void';
Boolean:                'boolean';
Number:                 'number';
String:                 'string';
Let:                    'let';
Const:                  'const';
If:                     'if';
Else:                   'else';
For:                    'for';
While:                  'while';
Break:                  'break';
Continue:               'continue';
Function:               'function';
Return:                 'return';

// 分隔符
OpenParen:              '(';
CloseParen:             ')';
OpenBrace:              '{';
CloseBrace:             '}';
Colon:                  ':';
SemiColon:              ';';
Comma:                  ',';

// 操作符
Assign:                 '=';
Plus:                   '+';
Minus:                  '-';
Multiply:               '*';
Divide:                 '/';
LessThan:               '<';
MoreThan:               '>';
LessThanEquals:         '<=';
GreaterThanEquals:      '>=';
Equals:                 '==';
NotEquals:              '!=';

// 字面量
BooleanLiteral:         'true' | 'false';
DecimalIntegerLiteral:  '0'
                     |  [1-9] [0-9]*
                     ;
StringLiteral:          '\'' SingleStringCharacter* '\'';
WhiteSpaces:            [\t\u000B\u000C\u0020\u00A0]+ -> channel(HIDDEN);
LineTerminator:         [\r\n\u2028\u2029] -> channel(HIDDEN);

// 标识符
Identifier:             Letter LetterOrDigit*;

// 注释
MultiLineComment:       '/*' .*? '*/'             -> channel(HIDDEN);
SingleLineComment:      '//' ~[\r\n\u2028\u2029]* -> channel(HIDDEN);

// Fragment规则
fragment SingleStringCharacter
    : ~['\\\r\n]
    | '\\' EscapeSequence
    | LineContinuation
    ;

fragment EscapeSequence
    : CharacterEscapeSequence
    ;

fragment CharacterEscapeSequence
    : SingleEscapeCharacter
    | NonEscapeCharacter
    ;

fragment SingleEscapeCharacter
    : ['"\\bfnrtv]
    ;

fragment NonEscapeCharacter
    : ~['"\\bfnrtv0-9xu\r\n]
    ;

fragment LineContinuation
    : '\\' [\r\n\u2028\u2029]
    ;

fragment LetterOrDigit
    : Letter
    | [0-9]
    ;

fragment Letter
    : [a-zA-Z$_]
    ;
