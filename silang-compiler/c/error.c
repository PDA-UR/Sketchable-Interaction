
#include "error.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void undefined_variable_error(char* var_name, void (*yyerror_ptr)(char*), int line_num)
{
    int size = (35 + strlen(var_name)) * sizeof(char) + 1;

    char* s = malloc(size);

    s[0] = '\0';

    sprintf(s, "identifier %s is not defined at line %d", var_name, line_num);

    s[size] = '\0';

    (*yyerror_ptr)(s);

    free(s);

    exit(1);
}

void illegal_link_assignment_shape_type_error(Variable* left, Variable* right, void (*yyerror_ptr)(char* s), int line_num)
{
    //illegal link assignment occurs when a shape has no type or blueprint type

    if(left->values._region._shape._type == 0 )
    {
        int size = (54 + 12) * sizeof(char) + 1;

        char* s = malloc(size);

        s[0] = '\0';

        sprintf(s, "Link cannot be assigned at line %d. Region has no shape.", line_num);

        s[size] = '\0';

        (*yyerror_ptr)(s);

        free(s);

        exit(1);
    }

    if(left->values._region._shape._type == TYPE_BLUEPRINT)
    {
        int size = (56 + 12) * sizeof(char) + 1;

        char* s = malloc(size);

        s[0] = '\0';

        sprintf(s, "Link cannot be assigned at line %d. Region is a Blueprint.", line_num);

        s[size] = '\0';

        (*yyerror_ptr)(s);

        free(s);

        exit(1);
    }
}