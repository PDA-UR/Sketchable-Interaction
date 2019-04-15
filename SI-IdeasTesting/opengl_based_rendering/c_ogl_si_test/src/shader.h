
#ifndef C_OGL_SI_TEST_SHADER_H
#define C_OGL_SI_TEST_SHADER_H

#include <GL/glew.h>

typedef struct
{
    GLuint  program_id, vertex_shader_id, fragment_shader_id;
    int num_attributes;

} Shader;

Shader shader_construct();
void shader_compile(Shader* shader, const char* vertex_shader_file_path, const char* fragment_shader_file_path);
void shader_link(Shader* shader);
void shader_add_attribute(Shader* shader, const char* attribute_name);

void shader_use(Shader* shader);
void shader_unuse(Shader* shader);

GLuint shader_get_uniform_location(Shader* shader, const char* uniform_name);

const char* shader_load_file(const char* source);

#endif //C_OGL_SI_TEST_SHADER_H
