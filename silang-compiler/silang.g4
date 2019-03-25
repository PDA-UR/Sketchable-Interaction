grammar silang;

program
    : statement+
    ;

statement
    : expression operator expression end_of_statement
    | end_of_statement
    ;

expression
    : expression operator expression
    | identifier operator identifier
    | capability separator capability
    | identifier operator keyword
    | TYPE OPEN_PAREN identifier CLOSE_PAREN
    | TYPE OPEN_PAREN keyword CLOSE_PAREN
    | OPEN_SB expression CLOSE_SB
    | OPEN_SB CLOSE_SB
    | point
    | point separator expression
    | keyword
    ;

identifier
    : STRING
    ;

keyword
    : TYPE
    | REGION
    | PRESENT
    | BLUEPRINT
    | SHAPE
    ;

point
    : OPEN_PAREN NUMBER separator NUMBER CLOSE_PAREN
    | OPEN_PAREN NUMBER separator NUMBER CLOSE_PAREN separator
    ;

operator
    : UNI_LINK
    | BI_LINK
    | ASSIGN_PROPERTY
    | ASSIGN_ATTRIBUTE
    | ASSIGN_VALUE
    ;

capability
    : 'position' //needs dynamic addition of those as keyword or other rules
    ;

separator
    : ','
    ;

end_of_statement
    : ';'
    ;

NUMBER
    : [0-9]+
    ;

TYPE
    : 'type'
    ;

REGION
    : 'region'
    ;

PRESENT
    : 'present'
    ;

BLUEPRINT
    : 'blueprint'
    ;

SHAPE
    : 'shape'
    ;

OPEN_PAREN
    : '('
    ;

CLOSE_PAREN
    : ')'
    ;

OPEN_SB
    : '['
    ;

CLOSE_SB
    : ']'
    ;

UNI_LINK
    : '->'
    ;

BI_LINK
    : '<->'
    ;

ASSIGN_VALUE
    : ':='
    ;

ASSIGN_PROPERTY
    : '=>'
    ;

ASSIGN_ATTRIBUTE
    : '&'
    ;

STRING
    : [a-zA-Z]+
    ;

LineComment
    : '#' '~[\r\n]*' -> skip
    ;

WS
   : [ \r\n\t] -> skip
   ;