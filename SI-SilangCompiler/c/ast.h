
#ifndef SILANG_AST_H
#define SILANG_AST_H

typedef struct __Region Region;

/* point         */

typedef struct __Point Point;

struct __Point
{
    int x;
    int y;

    Point* next;
};

const char* point_to_string(Point* p);

/*****************/

/* list          */

// as linked list
typedef struct
{
    Point* p, *last, *first;

    int size;
} List;

void allocate_list(List** list);
void add(List* list, Point p);
const char* list_to_string(List* list);
void free_point(Point* p);
void free_list(List* list);

/*****************/

/* shape         */

#define TYPE_BLUEPRINT 1
#define TYPE_PRESENT 2

typedef struct
{
    int _type;
    List _points;

} Shape;

/* Process Graph */

typedef struct __ProcessGraph ProcessGraph;
typedef struct __Variable Variable;

/*****************/

const char* shape_to_string(Shape* shape);
const char* shape_type_to_string(int type);

/* region        */

struct __ProcessGraph
{
    Variable* list, * first, * last;

    int size;

};

struct __Region
{
    char *_type;
    Shape _shape;
    ProcessGraph pg;
};
/*****************/


/* Process Graph */

const char* region_to_string(Region* region);
/*****************/

/* variable      */

#define REGION_VARIABLE 1
#define PROCESS_GRAPH_VARIABLE 2

typedef union
{
    Region _region;
    ProcessGraph pg;

} VariableValues;

struct __Variable
{
    char* name;
    VariableValues values;
    ProcessGraph pg;
    Variable* next;

    int value_num;
};

/*****************/

const char* variable_to_string(Variable* var);

/* assignment    */

typedef struct
{
    Variable var;
    Region _region;

} RegionAssignment;

const char* region_assignment_to_string(RegionAssignment* ra);

/*****************/

/* Link          */

#define UNIDIRECTIONAL_LINK 1
#define BIDIRECTIONAL_LINK 2

typedef struct
{
    char* left_var, *right_var;
    char* left_cap, *right_cap;

    int _type;
} Link;

const char* link_type_to_string(int type);
const char* link_to_string(Link* link);

/*****************/

#endif //SILANG_AST_H

/* expression    */

#define REGION_ASSIGNMENT 1
#define LINK_ASSIGNMENT 2

typedef struct
{
    union
    {
        RegionAssignment ra;
        Link link;

    } values;

    int value_num;

} Expression;

const char* expression_to_string(Expression* exp);

/*****************/