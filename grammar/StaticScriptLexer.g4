lexer grammar StaticScriptLexer;

options {
	language = Cpp;
}

// 关键字
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

// 一元操作符
PlusPlus:               '++';   // number->number
MinusMinus:             '--';   // number->number
Not:                    '!';    // boolean->boolean
BitNot:                 '~';    // number->number

// 二元操作符
Plus:                   '+';    // (number|string)->(number|string): Plus既是一元操作符也是二元操作符
Minus:                  '-';    // number->number: Minus既是一元操作符也是二元操作符
Multiply:               '*';    // number->number
Divide:                 '/';    // number->number
Modulus:                '%';    // number->number
BitAnd:                 '&';    // number->number
BitXOr:                 '^';    // number->number
BitOr:                  '|';    // number->number
LeftShift:              '<<';   // number->number
RightShift:             '>>';   // number->number
LessThan:               '<';    // number->boolean
GreaterThan:            '>';    // number->boolean
LessThanEquals:         '<=';   // number->boolean
GreaterThanEquals:      '>=';   // number->boolean
Equals:                 '==';   // (number|boolean|string)->boolean
NotEquals:              '!=';   // (number|boolean|string)->boolean
And:                    '&&';   // boolean->boolean
Or:                     '||';   // boolean->boolean
Assign:                 '=';    // (number|boolean|string)->(number|boolean|string)
PlusAssign:             '+=';   // (number|string)->(number|string)
MinusAssign:            '-=';   // number->number
MultiplyAssign:         '*=';   // number->number
DivideAssign:           '/=';   // number->number
ModulusAssign:          '%=';   // number->number
BitAndAssign:           '&=';   // number->number
BitXorAssign:           '^=';   // number->number
BitOrAssign:            '|=';   // number->number
LeftShiftAssign:        '<<=';  // number->number
RightShiftAssign:       '>>=';  // number->number
AndAssign:              '&&=';  // boolean->boolean
OrAssign:               '||=';  // boolean->boolean

// 字面量
BooleanLiteral:         'true' | 'false';
IntegerLiteral:         '0' |  [1-9] [0-9]*;
StringLiteral:          '"' StringCharacter* '"';
WhiteSpaces:            [\t\u000B\u000C\u0020\u00A0]+ -> channel(HIDDEN);
LineTerminator:         [\r\n\u2028\u2029] -> channel(HIDDEN);

// 标识符
Identifier:             Letter LetterOrDigit*;

// 注释
MultiLineComment:       '/*' .*? '*/'             -> channel(HIDDEN);
SingleLineComment:      '//' ~[\r\n\u2028\u2029]* -> channel(HIDDEN);

// Fragment规则
fragment StringCharacter
    : ~["\\\r\n]
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

fragment Letter : [a-zA-Z_] ;
