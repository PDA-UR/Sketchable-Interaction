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

%code requires
{
#include "../ast.h"

void assign_point(Point* p, int x, int y);
void assign_list(List* dest, Point* current, List* others);
void assign_shape(Shape* s, int _type, List* points);
void assign_region(Region* r, char* type, Shape* s);
void assign_link(Expression* exp, char* leftv, char* rightv, char* leftc, char* rightc, int _link_type);
void assign_variable_region(Expression* exp, char* var_name, Region* r);
}

%union { char* strval; int num; int ivar; Point pt; List li; Shape sha; Region re; Expression expr;};

%token <ivar> identifier
%token <num> number
%token assign_property "=>"
%token assign_value ":="
%token assign_unidirectional_link "->"
%token assign_bidirectional_link "<->"
%token region "region"
%token shape "shape"
%token type "type"
%token present "present"
%token blueprint "blueprint"
%token <strval> capability

%type <pt> point
%type <li> list
%type <re> region_assignment
%type <sha> shape_assignment
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
    assign_variable_region(&$$, vars[$1], &$3);
}

| identifier "->" identifier "=>" capability ',' capability
{
    assign_link(&$$, vars[$1], vars[$3], strdup($5), strdup($7), UNIDIRECTIONAL_LINK);
}

| identifier "->" identifier "=>" capability
{
    assign_link(&$$, vars[$1], vars[$3], strdup($5), strdup($5), UNIDIRECTIONAL_LINK);
}

| identifier "<->" identifier "=>" capability ',' capability
{
    assign_link(&$$, vars[$1], vars[$3], strdup($5), strdup($7), BIDIRECTIONAL_LINK);
}

| identifier "<->" identifier "=>" capability
{
    assign_link(&$$, vars[$1], vars[$3], strdup($5), strdup($5), BIDIRECTIONAL_LINK);
}
;

region_assignment
:  region "=>" type '(' identifier ')'
{
    assign_region(&$$, strdup(vars[$5]), NULL);
}

| region "=>" type '(' identifier ')' '&' shape_assignment
{
    assign_region(&$$, strdup(vars[$5]), &$8);
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
        if (strcmp(var, vars[i]) == 0)
            return i;

    vars[nvars] = strdup(var);

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

void assign_region(Region* r, char* _type, Shape* s)
{
    r->_type = _type;

    if(s != NULL)
        r->_shape = *s;
}

void assign_link(Expression* exp, char* leftv, char* rightv, char* leftc, char* rightc, int _link_type)
{
    // check if leftv and rightv are defined

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

    RegionAssignment ra;

    ra.var = var;
    ra._region = *r;

    exp->values.ra = ra;
    exp->value_num = REGION_ASSIGNMENT;
}