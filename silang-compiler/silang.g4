grammar silang;

program
    : expression* SEMICOLON
    ;

expression
    : identifier UNI_LINK identifier ASSIGN_PROPERTY capability COMMA capability
    | identifier BI_LINK identifier ASSIGN_PROPERTY capability COMMA capability
    | identifier ASSIGN_VALUE expression
    | REGION ASSIGN_PROPERTY TYPE OPEN_PAREN identifier CLOSE_PAREN
    | REGION ASSIGN_PROPERTY TYPE OPEN_PAREN identifier CLOSE_PAREN ASSIGN_ATTRIBUTE expression
    | present_shape_assignment expression
    | blueprint_shape_assignment expression
    | OPEN_SB CLOSE_SB
    | OPEN_SB expression CLOSE_SB
    | point COMMA expression
    | point
    | identifier
    | SEMICOLON
    ;

present_shape_assignment
    : SHAPE ASSIGN_PROPERTY TYPE OPEN_PAREN PRESENT CLOSE_PAREN ASSIGN_VALUE
    ;

blueprint_shape_assignment
    :SHAPE ASSIGN_PROPERTY TYPE OPEN_PAREN BLUEPRINT CLOSE_PAREN ASSIGN_VALUE
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
    : OPEN_PAREN NUMBER COMMA NUMBER CLOSE_PAREN
    | OPEN_PAREN NUMBER COMMA NUMBER CLOSE_PAREN COMMA
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

COMMA
    : ','
    ;

SEMICOLON
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