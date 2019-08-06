

#ifndef SITEST_REGIONSPRITE_H
#define SITEST_REGIONSPRITE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <GL/glew.h>

typedef struct RegionSprite_
{
    int x;
    int y;
    int width;
    int height;
    GLuint vbo_id;
} RegionSprite;

void region_sprite_initialize(RegionSprite* rs, int x, int y, int width, int height);
void region_sprite_draw(RegionSprite* rs);
void region_sprite_destroy(RegionSprite* rs);

#ifdef __cplusplus
};
#endif

#endif //SITEST_REGIONSPRITE_H
