
#include "RegionSprite.h"
#include "render_engine/sdl2/geometry/Vertex.h"
#include <stddef.h>

void region_sprite_initialize(RegionSprite* rs, int x, int y, int width, int height)
{
    rs->x = x;
    rs->y = y;
    rs->width = width;
    rs->height = height;

    glGenBuffers(1, &rs->vbo_id);

    Vertex vertex_data[6];

    vertex_data[0].position.x = x + width;
    vertex_data[0].position.y = y + height;
    vertex_data[1].position.x = x;
    vertex_data[1].position.y = y + height;
    vertex_data[2].position.x = x;
    vertex_data[2].position.y = y;
    vertex_data[3].position.x = x;
    vertex_data[3].position.y = y;
    vertex_data[4].position.x = x + width;
    vertex_data[4].position.y = y;
    vertex_data[5].position.x = x + width;
    vertex_data[5].position.y = y + height;

    int i;
    for(i = 0; i < 6; i++)
    {
        vertex_data[i].color.r = 255;
        vertex_data[i].color.g = 0;
        vertex_data[i].color.b = 255;
        vertex_data[i].color.a = 255;
    }

    glBindBuffer(GL_ARRAY_BUFFER, rs->vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void region_sprite_draw(RegionSprite* rs)
{
    glBindBuffer(GL_ARRAY_BUFFER, rs->vbo_id);
    glEnableVertexAttribArray(0);

    //position attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));

    //color attribute pointer
    // GL_TRUE is normalization
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*) offsetof(Vertex, color));


    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void region_sprite_destroy(RegionSprite* rs)
{
    glDeleteBuffers(1, &rs->vbo_id);
    rs = NULL;
}