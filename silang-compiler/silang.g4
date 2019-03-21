grammar silang;

program
    : statement+
    ;

statement
    : expression operator expression end_of_statement
    | end_of_statement
    ;

expression
    :
    | identifier operator identifier
    | capability separator capability
    ;

identifier
    : STRING
    ;

operator
    : UNI_LINK
    | BI_LINK
    | ASSIGN_PROPERTY
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

UNI_LINK
    : '->'
    ;

BI_LINK
    : '<->'
    ;

ASSIGN_PROPERTY
    : '=>'
    ;

STRING
    : [a-zA-Z]+
    ;

LineComment
    :   '#' '~[\r\n]*' -> skip
    ;

WS
   : '[ \r\n\t]' -> skip
   ;