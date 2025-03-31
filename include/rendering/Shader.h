#ifndef HIKAGE_SHADER_H
#define HIKAGE_SHADER_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "config/Project.h"
#include <string>
#include <map>

class Shader {
public:
    Shader();
    ~Shader();

    void build(Project* project);

    void bind() const;
    static void unbind();

    bool error();
    char* getLog();

    void uniform1li(const std::string &name, int x);
    void uniform1iv(const std::string &name, GLsizei count, const GLint* value);
    void uniform1lf(const std::string &name, float x);
    void uniform2fv(const std::string &name, glm::vec2 v);
    void uniform3fv(const std::string &name, glm::vec3 v);
    void uniform4fv(const std::string &name, glm::vec4 v);
    void uniformM4fv(const std::string &name, glm::mat4x4 mvp);

    void compile(std::string fs);
private:

    int compile_shader(GLenum type, const std::string &source);
    void create_program(GLuint vs, GLuint fs);

    int uniform_location(const std::string &name);

    GLuint _id = 0;
    GLuint _vs = 0;
    GLuint _fs = 0;
    std::map<std::string, GLint> _uniforms;

    bool _error = false;
    char* _err_log = nullptr;
    int _err_log_idx = 0;

    std::string vsSrc =
            "#version 430\n"
            "\n"
            "layout(location = 0) in vec4 position;\n"
            "\n"
            "void main() {\n"
            "    gl_Position = position;\n"
            "}";
};


#endif //HIKAGE_SHADER_H
