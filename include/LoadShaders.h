#ifndef __LOAD_SHADERS_H__
#define __LOAD_SHADERS_H__

typedef struct {
    GLenum       type;
    const char*  filename;
    GLuint       shader;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo*);

#endif
