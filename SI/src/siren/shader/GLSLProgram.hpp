

#ifndef SITEST_GLSLPROGRAM_HPP
#define SITEST_GLSLPROGRAM_HPP

#include <GL/glew.h>
#include <string>

class GLSLProgram
{
public:
    GLSLProgram();
    ~GLSLProgram();

    void use();
    void unuse();

    GLint uniform_location(const std::string &uniform_name);

    void compile_shaders(const std::string &vertex_shader_filepath, const std::string &fragment_shader_filepath);
    void link_shaders();
    void add_attribute(const std::string &attribute_name);

private:
    void compile_shader(const std::string &file_path, GLuint id);

    GLuint d_program_id;
    GLuint d_vertex_shader_id;
    GLuint d_fragment_shader_id;

    int d_num_attributes;
};


#endif //SITEST_GLSLPROGRAM_HPP
