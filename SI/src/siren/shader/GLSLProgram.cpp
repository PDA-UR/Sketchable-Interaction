
#include "GLSLProgram.hpp"
#include <fstream>
#include <iostream>

GLSLProgram::GLSLProgram() :
        d_fragment_shader_id(0),
        d_vertex_shader_id(0),
        d_program_id(0),
        d_num_attributes(0)
{}

GLSLProgram::~GLSLProgram()
{}

void GLSLProgram::use()
{
    glUseProgram(d_program_id);

    for(int i = 0; i < d_num_attributes; i++)
        glEnableVertexAttribArray(i);
}

void GLSLProgram::unuse()
{
    glUseProgram(0);

    for(int i = 0; i < d_num_attributes; i++)
        glDisableVertexAttribArray(i);
}

GLint GLSLProgram::uniform_location(const std::string &uniform_name)
{
    GLint location = glGetUniformLocation(d_program_id, uniform_name.c_str());

    if(location == GL_INVALID_INDEX)
    {
        std::cout << "invalid index for uniform: " << uniform_name << std::endl;
        exit(-1);
    }

    return location;
}

void GLSLProgram::compile_shaders(const std::string &vertex_shader_filepath,
                                  const std::string &fragment_shader_filepath)
{
    d_program_id = glCreateProgram();
    d_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    if(d_vertex_shader_id == 0)
    {
        std::cout << "vertex shader id not set" << std::endl;
        exit(-1);
    }

    d_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    if(d_fragment_shader_id == 0)
    {
        std::cout << "fragment shader id not set" << std::endl;
        exit(-1);
    }

    compile_shader(vertex_shader_filepath, d_vertex_shader_id);
    compile_shader(fragment_shader_filepath, d_fragment_shader_id);
}

void GLSLProgram::link_shaders()
{
    glAttachShader(d_program_id, d_vertex_shader_id);
    glAttachShader(d_program_id, d_fragment_shader_id);

    glLinkProgram(d_program_id);

    GLint is_linked = 0;
    glGetProgramiv(d_program_id, GL_LINK_STATUS, &is_linked);

    if(!is_linked)
    {
        GLint max_length = 0;
        glGetProgramiv(d_program_id, GL_INFO_LOG_LENGTH, &max_length);
        char log[max_length];
        glGetProgramInfoLog(d_program_id, max_length, &max_length, log);

        glDeleteProgram(d_program_id);
        glDeleteShader(d_vertex_shader_id);
        glDeleteShader(d_fragment_shader_id);

        std::cout << "Shader Error: " << log << std::endl;
        exit(-1);
    }

    glDetachShader(d_program_id, d_vertex_shader_id);
    glDetachShader(d_program_id, d_fragment_shader_id);
}

void GLSLProgram::add_attribute(const std::string &attribute_name)
{
    glBindAttribLocation(d_program_id, d_num_attributes++, attribute_name.c_str());
}

void GLSLProgram::compile_shader(const std::string &file_path, GLuint id)
{
    std::ifstream shader_file(file_path);

    if(shader_file.fail())
    {
        std::cout << "Shader Error: " << file_path << std::endl;
        exit(-1);
    }

    std::string file_contents = "";
    std::string line;

    while(std::getline(shader_file, line))
        file_contents += line + "\n";

    shader_file.close();

    const char* source = file_contents.c_str();

    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        GLint max_length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_length);

        char log[max_length];

        glGetShaderInfoLog(id, max_length, &max_length, log);
        glDeleteShader(id);

        std::cout << "Shader Error: " << log << std::endl;
        exit(-1);
    }
}