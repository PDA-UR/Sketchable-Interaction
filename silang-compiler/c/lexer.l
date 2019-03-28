%{
#include "parser.h"
#include <stdio.h>

int varindex(char *var);
%}

%%

"region"                                    {return region;}
"shape"                                     {return shape;}
"type"                                      {return type;}
"present"                                   {return present;}
"blueprint"                                 {return blueprint;}

[a-zA-Z][a-zA-Z0-9]*                        {yylval.ivar = varindex(strdup(yytext)); return identifier;}
[0-9]+                                      {yylval.num = atoi(yytext); return number;}
[ \t\n\r]                                   ;
"=>"                                        {return assign_property;}
":="                                        {return assign_value;}
[,;&()\[\]]                               {return yytext[0];}
.                                           {yyerror("unexpected character");}

%%

int yywrap(void)
{
    return 1;
}
