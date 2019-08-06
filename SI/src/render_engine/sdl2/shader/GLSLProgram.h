

#ifndef SITEST_GLSLPROGRAM_H
#define SITEST_GLSLPROGRAM_H


#ifdef __cplusplus
extern "C" {
#endif

#include <GL/glew.h>


typedef struct GLSLProgram_
{
    GLuint program_id;
    GLuint vertex_shader_id;
    GLuint fragment_shader_id;

    int num_attributes;

} GLSLProgram;

void glslprogram_use(GLSLProgram* prog);
void glslprogram_unuse(GLSLProgram* prog);

void glslprogram_initialize(GLSLProgram* prog);
void glslprogram_compile_shaders(GLSLProgram* prog, const char* vertex_shader_filepath, const char* fragment_shader_filepath);
void glslprogram_link_shaders(GLSLProgram* prog);
void glslprogram_add_attribute(GLSLProgram* prog, const char* name);

void __glslprogram_compile_shader(GLSLProgram* prog, const char* filepath, const GLuint* id);

#ifdef __cplusplus
};
#endif

#endif //SITEST_GLSLPROGRAM_H
