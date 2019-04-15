
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shader.h"

Shader shader_construct()
{
    Shader shader;

    shader.program_id = 0;
    shader.vertex_shader_id = 0;
    shader.fragment_shader_id = 0;
    shader.num_attributes = 0;

    return shader;
}

void shader_compile(Shader* shader, const char* vertex_shader_file_path, const char* fragment_shader_file_path)
{
    GLchar *log = NULL;
    GLint log_length, success;

    /* Vertex shader */
    shader->vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* vert = shader_load_file(vertex_shader_file_path);
    const GLchar* frag = shader_load_file(fragment_shader_file_path);

    glShaderSource(shader->vertex_shader_id, 1, &vert, NULL);
    glCompileShader(shader->vertex_shader_id);
    glGetShaderiv(shader->vertex_shader_id, GL_COMPILE_STATUS, &success);
    glGetShaderiv(shader->vertex_shader_id, GL_INFO_LOG_LENGTH, &log_length);

    log = (GLchar*) malloc(log_length);

    if (log_length > 0)
    {
        glGetShaderInfoLog(shader->vertex_shader_id, log_length, NULL, log);
        printf("vertex shader log:\n\n%s\n", log);
    }

    if (!success)
    {
        printf("vertex shader compile error\n");
        exit(EXIT_FAILURE);
    }

    /* Fragment shader */
    shader->fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(shader->fragment_shader_id, 1, &frag, NULL);
    glCompileShader(shader->fragment_shader_id);
    glGetShaderiv(shader->fragment_shader_id, GL_COMPILE_STATUS, &success);
    glGetShaderiv(shader->fragment_shader_id, GL_INFO_LOG_LENGTH, &log_length);

    if (log_length > 0)
    {
        log = (GLchar*) realloc(log, log_length);
        glGetShaderInfoLog(shader->fragment_shader_id, log_length, NULL, log);
        printf("fragment shader log:\n\n%s\n", log);
    }

    if (!success)
    {
        printf("fragment shader compile error\n");
        exit(EXIT_FAILURE);
    }
}

void shader_link(Shader* shader)
{
    shader->program_id = glCreateProgram();

    glAttachShader(shader->program_id, shader->vertex_shader_id);
    glAttachShader(shader->program_id, shader->fragment_shader_id);
    glLinkProgram(shader->program_id);

    glDeleteShader(shader->vertex_shader_id);
    glDeleteShader(shader->fragment_shader_id);
}

void shader_add_attribute(Shader* shader, const char* attribute_name)
{
    glBindAttribLocation(shader->program_id, shader->num_attributes++, attribute_name);
}

void shader_use(Shader* shader)
{
    glUseProgram(shader->program_id);

    int i;
    for(i = 0; i < shader->num_attributes; i++)
        glEnableVertexAttribArray(i);
}

void shader_unuse(Shader* shader)
{
    glUseProgram(0);

    int i;
    for(i = 0; i < shader->num_attributes; i++)
        glDisableVertexAttribArray(i);
}

GLuint shader_get_uniform_location(Shader* shader, const char* uniform_name)
{
    GLuint location = glGetUniformLocation(shader->program_id, uniform_name);

    if(location == GL_INVALID_INDEX)
    {
        printf("Uniform %s not found in shader", uniform_name);
        exit(1);
    }

    return location;
}

const char* shader_load_file(const char* source)
{
    char * buffer = NULL;
    long length;

    FILE * f = fopen(source, "rb");

    if(f)
    {
        fseek(f, 0, SEEK_END);

        length = ftell(f);

        fseek(f, 0, SEEK_SET);

        buffer = (char*) malloc(length);

        if (buffer)
        {
            buffer[0] = '\0';

            fread(buffer, 1, length, f);

            buffer[length] = '\0';
        }

        fclose(f);
    }

    char* ret = strdup(buffer);

    free(buffer);

    return ret;
}