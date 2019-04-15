//
// Created by juergen on 03/04/19.
//

#include "rect.h"
#include <stddef.h>

Rect rect_construct(float x, float y, float width, float height)
{
    Rect rect;

    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;

    glGenBuffers(1, &rect.vboID);

    Vertex vertices[6];

    vertices[0].position.x = x + width;
    vertices[0].position.y = y + height;

    vertices[1].position.x = x;
    vertices[1].position.y = y + height;

    vertices[2].position.x = x;
    vertices[2].position.y = y;

    vertices[3].position.x = x;
    vertices[3].position.y = y;

    vertices[4].position.x = x + width;
    vertices[4].position.y = y;

    vertices[5].position.x = x + width;
    vertices[5].position.y = y + height;

    int i;

    for(i = 0; i < 6; i++)
    {
        vertices[i].color.r = 255;
        vertices[i].color.g = 0;
        vertices[i].color.b = 255;
        vertices[i].color.a = 255;
    }

    glBindBuffer(GL_ARRAY_BUFFER, rect.vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return rect;
}

void rect_destruct(Rect* rect)
{

}

void rect_render(Rect* rect)
{
    glBindBuffer(GL_ARRAY_BUFFER, rect->vboID);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*) offsetof(Vertex, color));



    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}