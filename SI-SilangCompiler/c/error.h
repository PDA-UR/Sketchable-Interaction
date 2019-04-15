
#ifndef SILANG_ERROR_H
#define SILANG_ERROR_H

#include "ast.h"

void undefined_variable_error(char* var_name, void (*yyerror_ptr)(char* s), int line_num);
void illegal_link_assignment_shape_type_error(Variable* left, Variable* right, void (*yyerror_ptr)(char* s), int line_num);

#endif //SILANG_ERROR_H
