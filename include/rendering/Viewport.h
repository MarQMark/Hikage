#ifndef HIKAGE_VIEWPORT_H
#define HIKAGE_VIEWPORT_H


#include "GL/glew.h"
#include "Uniforms.h"
#include "Shader.h"

class Viewport {
public:
    Viewport(int width, int height);
    ~Viewport();

    void update(int width, int height);

    void render();
    GLuint txt() const;

    void bind(GLuint slot = 0) const;
    static void unbind();

    void setShader(Shader* shader);

private:
    void gen_txt();

    int _width = 0;
    int _height = 0;

    GLuint _fbo = 0;
    GLuint _txt = 0;
    GLuint _vb = 0;

    Shader* _shader;
};


#endif //HIKAGE_VIEWPORT_H
