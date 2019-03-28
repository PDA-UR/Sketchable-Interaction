%{
#include "parser.h"
#include <stdio.h>

int varindex(char *var);
%}

%%

[a-zA-Z][a-zA-Z0-9]*            {yylval.ivar = varindex(yytext); return identifier;}
[ \t\n\r\s]                     ;
[',', ';', '=', '&']            {return yytext[0];}
.                               {yyerror("unexpected character");}

%%

int yywrap(void)
{
    return 1;
}
