

#include "GLSLProgram.h"
#include <stdio.h>
#include <stdlib.h>

void glslprogram_use(GLSLProgram* prog)
{
    glUseProgram(prog->program_id);

    int i;
    for(i = 0; i < prog->num_attributes; i++)
        glEnableVertexAttribArray(i);
}

void glslprogram_unuse(GLSLProgram* prog)
{
    glUseProgram(0);

    int i;
    for(i = 0; i < prog->num_attributes; i++)
        glDisableVertexAttribArray(i);
}

void glslprogram_initialize(GLSLProgram* prog)
{
    prog->fragment_shader_id = 0;
    prog->vertex_shader_id = 0;
    prog->program_id = 0;
    prog->num_attributes = 0;
}

void glslprogram_compile_shaders(GLSLProgram* prog, const char* vertex_shader_filepath, const char* fragment_shader_filepath)
{
    prog->vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    if(prog->vertex_shader_id == 0)
        fprintf(stderr, "Vertex shader failed to be created\n");

    prog->fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    if(prog->fragment_shader_id == 0)
       fprintf(stderr, "Fragment shader failed to be created!\n");

    __glslprogram_compile_shader(prog, vertex_shader_filepath, &prog->vertex_shader_id);
    __glslprogram_compile_shader(prog, fragment_shader_filepath, &prog->fragment_shader_id);
}

void glslprogram_link_shaders(GLSLProgram* prog)
{
    prog->program_id = glCreateProgram();

    glAttachShader(prog->program_id, prog->vertex_shader_id);
    glAttachShader(prog->program_id, prog->fragment_shader_id);

    glLinkProgram(prog->program_id);

    GLint is_linked = 0;
    glGetProgramiv(prog->program_id, GL_LINK_STATUS, &is_linked);

    if(!is_linked)
    {
        GLint max_length = 0;
        glGetProgramiv(prog->program_id, GL_INFO_LOG_LENGTH, &max_length);
        char log[max_length];
        glGetProgramInfoLog(prog->program_id, max_length, &max_length, log);

        fprintf(stderr, "Unable to link shaders\n");
        fprintf(stderr, "%s\n", log);

        glDeleteProgram(prog->program_id);
        glDeleteShader(prog->vertex_shader_id);
        glDeleteShader(prog->fragment_shader_id);
    }

    glDetachShader(prog->program_id, prog->vertex_shader_id);
    glDetachShader(prog->program_id, prog->fragment_shader_id);
}

void glslprogram_add_attribute(GLSLProgram* prog, const char* name)
{
    glBindAttribLocation(prog->program_id, prog->num_attributes++, name);
}

void __glslprogram_compile_shader(GLSLProgram* prog, const char* filepath, const GLuint* id)
{
    char* buffer = 0;
    int length;

    FILE* file = fopen(filepath, "rb");

    if(file)
    {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);

        buffer = (char *) malloc(length);

        if(buffer)
        {
            fread(buffer, 1, length, file);
        }

        fclose(file);
    }

    buffer[length] = '\0';

    printf("%s\n", buffer);

    glShaderSource(*id, 1, &buffer, NULL);
    glCompileShader(*id);

    free(buffer);

    GLint success = 0;
    glGetShaderiv(*id, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        GLint max_length = 0;
        glGetShaderiv(*id, GL_INFO_LOG_LENGTH, &max_length);

        char log[max_length];

        glGetShaderInfoLog(*id, max_length, &max_length, log);
        glDeleteShader(*id);

        printf("%s\n", log);
        fprintf(stderr, "Shader %s failed to compile", filepath);
    }
}
