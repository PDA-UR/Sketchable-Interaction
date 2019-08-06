
#include "RegionSprite.h"

void region_sprite_initialize(RegionSprite* rs, int x, int y, int width, int height)
{
    rs->x = x;
    rs->y = y;
    rs->width = width;
    rs->height = height;

    glGenBuffers(1, &rs->vbo_id);

    float vertex_data[12];

    vertex_data[0] = x + width;
    vertex_data[1] = y + height;
    vertex_data[2] = x;
    vertex_data[3] = y + height;
    vertex_data[4] = x;
    vertex_data[5] = y;
    vertex_data[6] = x;
    vertex_data[7] = y;
    vertex_data[8] = x + width;
    vertex_data[9] = y;
    vertex_data[10] = x + width;
    vertex_data[11] = y + height;

    glBindBuffer(GL_ARRAY_BUFFER, rs->vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void region_sprite_draw(RegionSprite* rs)
{
    glBindBuffer(GL_ARRAY_BUFFER, rs->vbo_id);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void region_sprite_destroy(RegionSprite* rs)
{
    glDeleteBuffers(1, &rs->vbo_id);
    rs = NULL;
}