#ifndef HIKAGE_ICON_H
#define HIKAGE_ICON_H

#include <string>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Shader.h"

class Icon{
public:
    Icon();
    ~Icon();

    void gen(GLFWwindow* window, const std::string& text);

private:
    GLuint _fbo;
    GLuint _txt;
    GLuint _vb;

    Shader _shader;
};

void setIcon(GLFWwindow* window, const std::string& name);

#endif //HIKAGE_ICON_H
