

#include <render_engine/sdl2/lib/picoPNG.hpp>
#include <render_engine/sdl2/io/IOManager.h>
#include <cstdlib>
#include "ImageLoader.hpp"

GLTexture ImageLoader::load_png(const char *filepath)
{
    GLTexture texture = {};

    std::vector<unsigned char> out;
    unsigned long width, height;
    char* in;
    int in_size;

    if(!iomanager_read_file_to_buffer(in, &in_size, filepath))
    {
        // error handling
        exit(1);
    }

    if(in)
    {
        int error = decodePNG(out, width, height, (unsigned char*) in, in_size);

        if(error)
        {
            // error handling
            exit(1);
        }

        glGenTextures(1, &(texture.id));
        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        // error handling
        exit(1);
    }

    return texture;
}
