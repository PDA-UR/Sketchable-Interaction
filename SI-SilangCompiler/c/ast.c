
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

/* point         */

const char* point_to_string(Point* p)
{
    char x[12], y[12];

    sprintf(x, "%d", (int) p->x);
    sprintf(y, "%d", (int) p->y);

    int size = (3 + strlen(x) + strlen(y)) * sizeof(char) + 1;

    char* dest = malloc(size);

    dest[0] = '\0';

    strcat(strcat(strcat(strcat(strcat(dest, "("), x), ","), y), ")");

    dest[size] = '\0';

    char* ret = strdup(dest);

    free(dest);

    return ret;
}

/*****************/

/* list          */

void allocate_list(List** list)
{
    if(list == NULL) return;

    *list = (List*) malloc(sizeof(List));
    (*list)->last = NULL;
    (*list)->first = NULL;
    (*list)->size = 0;
}

void add(List* list, Point p)
{
    if(list == NULL) return;

    Point* q = (Point*) malloc(sizeof(Point));

    if(list->last == NULL)
    {
        list->last = q;
        list->first = q;
        list->size = 1;
    }
    else
    {
        list->last->next = q;
        list->size++;
    }

    *q = p;
    q->next = NULL;
    list->last = q;
}

const char* list_to_string(List* list)
{
    int num_points = list->size;
    int num_commas = num_points - 1;

    int size = (2 + num_points * 27 + num_commas) * sizeof(char) + 1;

    Point* p = list->first;

    char* s = malloc(size);
    s[0] = '\0';

    strcat(s, "[");

    while(p != NULL)
    {
        if(p == list->last)
            strcat(s, point_to_string(p));
        else
            strcat(strcat(s, point_to_string(p)), ",");

        p = p->next;
    }

    strcat(s, "]");
    s[size] = '\0';

    char* ret = strdup(s);

    free(s);

    return ret;
}

void free_point(Point* p)
{
    if(p->next != NULL)
        free_point(p->next);

    free(p);
}

void free_list(List* list)
{
    if(list != NULL)
    {
        free_point(list->first);
        free(list);
    }
}

/*****************/

/* shape         */

const char* shape_to_string(Shape* shape)
{
    const char* type = shape_type_to_string(shape->_type);
    const char* list = list_to_string(&shape->_points);

    int size = (14 + strlen(type) + strlen(list)) * sizeof(char) + 1;

    char* s = malloc(size);
    s[0] = '\0';

    sprintf(s, "type(%s), List(%s)", type, list);

    s[size] = '\0';

    char* ret = strdup(s);

    free(s);

    return ret;
}

const char* shape_type_to_string(int type)
{
    switch (type)
    {
        case TYPE_BLUEPRINT:
            return "blueprint";
        case TYPE_PRESENT:
            return "present";
        default:
            return "";
    }
}


/*****************/

/* region        */

const char* region_to_string(Region* region)
{
    const char* type = region->_type;
    const char* shape = shape_to_string(&region->_shape);

    int size = (15 + strlen(type) + strlen(shape)) * sizeof(char) + 1;

    char* s = malloc(size);

    s[0] = '\0';

    sprintf(s, "type(%s), Shape(%s)", type, shape);

    s[size] = '\0';

    char* ret = strdup(s);

    free(s);

    return ret;
}

/*****************/

/* variable      */
const char* variable_to_string(Variable* var)
{
    switch (var->value_num)
    {
        case REGION_VARIABLE:
        {
            char* var_name = var->name;
            const char* region = region_to_string(&var->values._region);

            int size = (16 + strlen(var_name) + strlen(region)) * sizeof(char) + 1;

            char* s = malloc(size);

            s[0] = '\0';

            sprintf(s, "name(%s), Region(%s)", var_name, region);

            s[size] = '\0';

            char* ret = strdup(s);

            free(s);

            return ret;
        }

        default:
            return "";
    }
}

/*****************/

/* assignment    */

const char* region_assignment_to_string(RegionAssignment* ra)
{
    const char* var = variable_to_string(&ra->var);
    const char* region = region_to_string(&ra->_region);

    int size = (20 + strlen(var) + strlen(region)) * sizeof(char) + 1;

    char* s = malloc(size);

    s[0] = '\0';

    sprintf(s, "Variable(%s), Region(%s)", var, region);

    s[size] = '\0';

    char* ret = strdup(s);

    free(s);

    return ret;
}
/*****************/

/*link           */

const char* link_type_to_string(int type)
{
    switch (type)
    {
        case UNIDIRECTIONAL_LINK:
            return "UnidirectionalLink";
        case BIDIRECTIONAL_LINK:
            return "BidirectionalLink";
        default:
            return "";
    }
}

const char* link_to_string(Link* link)
{
    const char* type = link_type_to_string(link->_type);

    int size = (45 + strlen(type)
            + strlen(link->left_var)
            + strlen(link->right_var)
            + strlen(link->left_cap)
            + strlen(link->right_cap)) * sizeof(char) + 1;

    char* s = malloc(size);

    s[0] = '\0';

    sprintf(s, "Link(type(%s), variables(%s, %s), capabilities(%s, %s))",
            type, link->left_var, link->right_var, link->left_cap, link->right_cap);

    s[size] = '\0';

    char* ret = strdup(s);

    free(s);

    return ret;
}

/*****************/

/* expression    */

const char* expression_to_string(Expression* exp)
{
    switch (exp->value_num)
    {
        case REGION_ASSIGNMENT:
            return region_assignment_to_string(&exp->values.ra);
        case LINK_ASSIGNMENT:
            return link_to_string(&exp->values.link);
        default:
            return "unsupported";
    }
}
/*****************/