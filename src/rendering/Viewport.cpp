#include "rendering/Viewport.h"
#include "config/Project.h"

Viewport::Viewport(int width, int height) : _width(width), _height(height) {
    glGenFramebuffers(1, &_fbo);
    gen_txt();

    // Setup Vertexbuffer
    glGenBuffers(1, &_vb);
    glBindBuffer(GL_ARRAY_BUFFER, _vb);

    float v[] = {
            -1,  1, 0, 0,
             1,  1, 0, 0,
             1, -1, 0, 0,

            -1,  1, 0, 0,
             1, -1, 0, 0,
            -1, -1, 0, 0,
    };
    glBufferData(GL_ARRAY_BUFFER, 4*4 * 6, v, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4*4, 0);
}

Viewport::~Viewport() {
    glDeleteTextures(1, &_txt);
    glDeleteBuffers(1, &_vb);
    glDeleteFramebuffers(1, &_fbo);
}

void Viewport::gen_txt() {
    glGenTextures(1, &_txt);
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    unbind();
}

void Viewport::bind(GLuint slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _txt);
}

void Viewport::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Viewport::update(int width, int height) {
    _width = width;
    _height = height;
    glDeleteTextures(1, &_txt);

    if(Project::get())
        Project::get()->setDimensions(_width, _height);

    gen_txt();
}

GLuint Viewport::txt() const {
    return _txt;
}

void Viewport::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _txt, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _width, _height);

    // TODO: Add uniforms
    _shader->uniform1lf("u_time", _uniforms->tt);
    _shader->uniform1lf("u_delta_time", _uniforms->dt);
    _shader->uniform2fv("u_mouse", _uniforms->mousePos);
    _shader->uniform2fv("u_resolution", _uniforms->dim);

    glDrawArrays(GL_TRIANGLES, 0 , 6);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Viewport::setUniforms(Uniforms *uniforms) {
    _uniforms = uniforms;
}

void Viewport::setShader(Shader *shader) {
    _shader = shader;
}
