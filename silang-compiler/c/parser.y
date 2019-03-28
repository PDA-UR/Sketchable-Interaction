%{

int yylex(void);
void yyerror(char *s);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NVARS 100

char* vars[NVARS], vals[NVARS];

int nvars = 0;

%}

%union { char* strval; int num; int ivar;}

%token <ivar> identifier
%token <num> number
%token assign_property "=>"
%token assign_value ":="
%token region "region"
%token shape "shape"
%token type "type"
%token present "present"
%token blueprint "blueprint"

%type <strval> expression, point

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
                                                                        char* s = "region of type ";

                                                                        int size = (strlen(s) + strlen(vars[$5]) + strlen($8)) * sizeof(char) + 1;

                                                                        char* target = malloc(size);

                                                                        target[0] = '\0';

                                                                        strcat(strcat(strcat(target, s), vars[$5]), $8);

                                                                        target[size] = '\0';

                                                                        $$ = strdup(target);

                                                                        free(target);
                                                                    }

           | shape "=>" type '(' present ')' ":=" expression        {
                                                                        char* s = " shape of type present with points: ";

                                                                        int size = (strlen(s) + strlen($8)) * sizeof(char) + 1;

                                                                        char* target = malloc(size);

                                                                        target[0] = '\0';

                                                                        strcat(strcat(target, s), $8);

                                                                        target[size] = '\0';

                                                                        $$ = strdup(target);

                                                                        free(target);
                                                                    }

           | shape "=>" type '(' blueprint ')' ":=" expression      {
                                                                        char* s = " with shape of type present with points: ";

                                                                        int size = (strlen(s) + strlen($8)) * sizeof(char) + 1;

                                                                        char* target = malloc(size);

                                                                        target[0] = '\0';

                                                                        strcat(strcat(target, s), $8);

                                                                        target[size] = '\0';

                                                                        $$ = strdup(target);

                                                                        free(target);
                                                                    }

           | '[' ']'                                                {
                                                                        char* s = "[]";
                                                                        $$ = s;
                                                                    }

           | '[' expression ']'                                     {
                                                                        int size = (2 + strlen($2)) * sizeof(char) + 1;

                                                                        char* target = malloc(size);

                                                                        target[0] = '\0';

                                                                        strcat(strcat(strcat(target, "["), $2), "]");

                                                                        target[size] = '\0';
                                                                        $$ = strdup(target);

                                                                        free(target);
                                                                    }

           | point ',' expression                                   {
                                                                        int size = (strlen($1) + 1 + strlen($3)) * sizeof(char) + 1;

                                                                        char* target = malloc(size);

                                                                        target[0] = '\0';

                                                                        strcat(strcat(strcat(target, $1), ","), $3);

                                                                        target[size] = '\0';

                                                                        $$ = strdup(target);

                                                                        free(target);
                                                                    };

           | point                                                  {$$ = $1;}
           ;

point : '(' number ',' number ')'                                   {
                                                                        char x[12], y[12];

                                                                        sprintf(x, "%d", (int) $2);
                                                                        sprintf(y, "%d", (int) $4);

                                                                        int size = (3 + strlen(x) + strlen(y)) * sizeof(char) + 1;

                                                                        char *target = malloc(size);

                                                                        target[0] = '\0';

                                                                        strcat(strcat(strcat(strcat(strcat(target, "("), x), ","), y), ")");

                                                                        target[size] = '\0';

                                                                        $$ = strdup(target);

                                                                        free(target);
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