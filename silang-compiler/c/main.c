
#include "parser.h"
#include <stdio.h>

extern int yyparse();
extern FILE* yyin;

int main(int argc, char** argv)
{
    int i;

    for(i = 1; i < argc; i++)
    {
        yyin = fopen(argv[i], "r");

        do
            yyparse();
        while (!feof(yyin));

        fclose(yyin);
    }

    return 0;
}
