

#include <render_engine/sdl2/lib/picoPNG.hpp>
#include <render_engine/sdl2/io/IOManager.hpp>
#include <cstdlib>
#include <core/debug/Print.hpp>
#include "ImageLoader.hpp"

namespace SIRen
{
    GLTexture ImageLoader::load_png(const char *filepath)
    {
        GLTexture texture = {};

        std::vector<char> in;
        std::vector<unsigned char> out;
        unsigned long width, height;

        if (!IOManager::read_file_to_buffer(filepath, in))
        {
            // error handling
            exit(5);
        }

        if (!in.empty())
        {
            int error = decodePNG(out, width, height, (unsigned char *) in.data(), in.size());

            if (error)
            {

//                Print::print("Image Loader error", error);

                // error handling
                exit(2);
            }

            glGenTextures(1, &(texture.id));
            glBindTexture(GL_TEXTURE_2D, texture.id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, 0);

            texture.width = width;
            texture.height = height;
        }
        else
        {
            // error handling
            exit(3);
        }

        return texture;
    }
}