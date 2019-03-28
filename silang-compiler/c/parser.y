%{
void yyerror(char* s);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NVARS 100
char *vars[NVARS];
double vals[NVARS];
int nvars=0;
%}


%union { char *id; int num; int ivar; double dval;}

%token <ivar> identifier
%type <ivar> expression
%%

program : line ';' program                          {;}
        | line ';'                                  {;}
        ;

line : assignment_value                             {;}
     ;

assignment_value : identifier '=' expression        {vals[$1] = $3; printf("BBQ %s, ROFL %s\n", vars[$1], vars[$3]);}
                 ;

expression : identifier                             {$$ = $1;}
           ;


%%

int varindex(char* var)
{
    int i;
    for (i = 0; i < nvars; i++)
        if (strcmp(var, vars[i]) == 0)
            return i;

    vars[nvars] = strdup(var);

    return nvars++;
}

int main(void)
{
    yyparse();

    return 0;
}

void yyerror(char* s)
{
    fprintf(stderr, "ERROR: %s\n", s);
}