%{

extern int yylineno;

int yylex(void);
void yyerror(char *s);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

#define NVARS 1000

char* var_names[NVARS];

int nvars = 0;

%}


%code requires
{

#include "../ast.h"

Variable variables[1000];

void assign_point(Point* p, int x, int y);
void assign_list(List* dest, Point* current, List* others);
void assign_shape(Shape* s, int _type, List* points);
void assign_region(Region* r, char* _type, Shape* s, ProcessGraph* pg);
void assign_link(Expression* exp, char* leftv, char* rightv, char* leftc, char* rightc, int _link_type);
void assign_variable_region(Expression* exp, char* var_name, Region* r);
void assign_region_list(ProcessGraph* dest, char* varname, ProcessGraph* others);
void add_to_region_list(ProcessGraph* pg, char* varname);
}

%union
{
    char* strval;
    int num;
    int ivar;
    Variable var;
    Point pt;
    List li;
    Shape sha;
    Region re;
    ProcessGraph pg;
    Expression expr;
};

%token <ivar> identifier
%token <num> number
%token assign_property "=>"
%token assign_value ":="
%token assign_unidirectional_link "->"
%token assign_bidirectional_link "<->"
%token region "region"
%token sequence "sequence"
%token composition "composition"
%token shape "shape"
%token type "type"
%token present "present"
%token blueprint "blueprint"
%token <strval> capability

%type <pt> point
%type <li> list
%type <pg> process_graph_assignment
%type <pg> region_list
%type <re> region_assignment
%type <sha> shape_assignment
%type <strval> variable
%type <expr> expression

%%

program
: expression ';'
{
    printf("%s\n", expression_to_string(&$<expr>1));
}

| program expression ';'
{
    printf("%s\n", expression_to_string(&$<expr>2));
}
;

expression
: identifier ":=" region_assignment
{
    assign_variable_region(&$$, var_names[$1], &$3);
}

| identifier "->" identifier "=>" capability ',' capability
{
    assign_link(&$$, var_names[$1], var_names[$3], strdup($5), strdup($7), UNIDIRECTIONAL_LINK);
}

| identifier "->" identifier "=>" capability
{
    assign_link(&$$, var_names[$1], var_names[$3], strdup($5), strdup($5), UNIDIRECTIONAL_LINK);
}

| identifier "<->" identifier "=>" capability ',' capability
{
    assign_link(&$$, var_names[$1], var_names[$3], strdup($5), strdup($7), BIDIRECTIONAL_LINK);
}

| identifier "<->" identifier "=>" capability
{
    assign_link(&$$, var_names[$1], var_names[$3], strdup($5), strdup($5), BIDIRECTIONAL_LINK);
}
;

region_assignment
:  region "=>" type '(' identifier ')'
{
    assign_region(&$$, strdup(var_names[$5]), NULL, NULL);
}

| region "=>" type '(' identifier ')' '&' shape_assignment
{
    assign_region(&$$, strdup(var_names[$5]), &$8, NULL);
}
| region "=>" type '(' composition ')' '&' process_graph_assignment
{
    assign_region(&$$, "composition", NULL, &$8);
}
;

process_graph_assignment
: sequence ":=" '[' region_list ']'
{
    $$ = $4;
}
;

region_list
: variable ',' region_list
{
    assign_region_list(&$$, $1, &$3);
}
| variable
{
    add_to_region_list(&$$, $1);
}
|
{;}
;

variable
: identifier
{
    $$ = var_names[$1];
}
;

shape_assignment
: shape "=>" type '(' present ')' ":=" '[' list ']'
{
    assign_shape(&$$, TYPE_PRESENT, &$<li>9);
}
| shape "=>" type '(' blueprint ')' ":=" '[' list ']'
{
    assign_shape(&$$, TYPE_BLUEPRINT, &$<li>9);
}
;

list
: point ',' list
{
    assign_list(&$$, &$1, &$3);
}

| point
{
    add(&$<li>$, $1);
}

|
{;}
;

point
: '(' number ',' number ')'
{
    assign_point(&$$, $2, $4);
}
;

%%

int varindex(char* var)
{
    int i;
    for (i = 0; i < nvars; i++)
        if (strcmp(var, var_names[i]) == 0)
            return i;

    var_names[nvars] = strdup(var);

    return nvars++;
}

void yyerror(char* s)
{
    fprintf(stderr, "ERROR: %s\n", s);
}

void assign_point(Point* p, int x, int y)
{
    p->x = x;
    p->y = y;
}

void assign_region_list(ProcessGraph* dest, char* varname, ProcessGraph* others)
{
    add_to_region_list(dest, varname);

    Variable* v = others->first;

    while(v != NULL)
    {
        add_to_region_list(dest, v->name);

        v = v->next;
    }
}

void add_to_region_list(ProcessGraph* pg, char* varname)
{
    if(pg == NULL)
        return;

    Variable* var = (Variable*) malloc(sizeof(Variable));

    if(pg->last == NULL)
    {
        pg->last = var;
        pg->first = var;
        pg->size = 1;
    }
    else
    {
        pg->last->next = var;

        pg->size++;
    }

    *var = variables[varindex(varname)];

    if(var->value_num == 0)
        undefined_variable_error(varname, &yyerror, yylineno);

    var->next = NULL;
    pg->last = var;
}

void assign_list(List* dest, Point* current, List* others)
{
    add(dest, *current);

    Point* p = others->first;

    while(p != NULL)
    {
        add(dest, *p);

        p = p->next;
    }
}

void assign_shape(Shape* s, int _type, List* points)
{
    s->_type = _type;
    s->_points = *points;
}

void assign_region(Region* r, char* _type, Shape* s, ProcessGraph* pg)
{
    r->_type = _type;

    if(s != NULL)
        r->_shape = *s;

    if(pg != NULL)
        r->pg = *pg;
}

void assign_link(Expression* exp, char* leftv, char* rightv, char* leftc, char* rightc, int _link_type)
{
    if(variables[varindex(leftv)].value_num == 0)
        undefined_variable_error(leftv, &yyerror, yylineno);

    if(variables[varindex(rightv)].value_num == 0)
        undefined_variable_error(rightv, &yyerror, yylineno);

    illegal_link_assignment_shape_type_error(&variables[varindex(leftv)], &variables[varindex((rightv))], &yyerror, yylineno);

    exp->value_num = LINK_ASSIGNMENT;

    Link link;
    link.left_var = leftv;
    link.right_var = rightv;
    link.left_cap = leftc;
    link.right_cap = rightc;
    link._type = _link_type;

    exp->values.link = link;
}

void assign_variable_region(Expression* exp, char* var_name, Region* r)
{
    Variable var;

    var.name = var_name;

    var.values._region = *r;
    var.value_num = REGION_VARIABLE;
    var.pg = r->pg;

    variables[varindex(var_name)] = var;

    RegionAssignment ra;

    ra.var = var;
    ra._region = *r;

    exp->values.ra = ra;
    exp->value_num = REGION_ASSIGNMENT;
}