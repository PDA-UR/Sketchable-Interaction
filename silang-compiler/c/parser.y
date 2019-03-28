%{
int yylex(void);
void yyerror(char *s);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NVARS 100

char* vars[NVARS];
char* vals[NVARS];
int nvars = 0;


%}

%union { char* strval; int num; int ivar; double dval;}

%token <ivar> identifier
%token assign_property "=>"
%token assign_value ":="
%token region "region"
%token shape "shape"
%token type "type"
%token present "present"
%token blueprint "blueprint"

%type <strval> expression

%%

program : expression ';'                                            {;}
        | program expression ';'                                    {;}
        ;

expression : identifier ":=" expression                             {printf("variable %s, assigned %s\n", vars[$1], $3);}
           | identifier                                             {$$ = vars[$1];}
           | region "=>" type '(' identifier ')'                    {
                                                                        char* s = "region of type ";

                                                                        int size = (strlen(s) + strlen(vars[$5])) * sizeof(char) + 1;

                                                                        char* target = malloc(size);

                                                                        target[0] = '\0';

                                                                        strcat(strcat(target, s), vars[$5]);

                                                                        target[size] = '\0';

                                                                        $$ = strdup(target);

                                                                        free(target);
                                                                    }

           | region "=>" type '(' identifier ')' '&' expression     {
                                                                        $$ = $8; printf("new exp: %s", $8);
                                                                    }

           | shape "=>" type '(' present ')' ":=" expression        {
                                                                        $$ = $8;
                                                                    }

           | shape "=>" type '(' blueprint ')' ":=" expression      {

                                                                    }

           | '[' ']'                                                {
                                                                        char* s = "[]";
                                                                        $$ = s;
                                                                    }

           | '[' expression ']'                                     {

                                                                    }
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

void yyerror(char* s)
{
    fprintf(stderr, "ERROR: %s\n", s);
}