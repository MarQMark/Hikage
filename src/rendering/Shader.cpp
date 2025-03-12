#include <iostream>
#include <cstring>
#include <fstream>
#include "rendering/Shader.h"

#define SHADER_ERR_LOG_SIZE 1024

Shader::Shader() {
    _err_log = (char*)malloc(SHADER_ERR_LOG_SIZE);
    memset(_err_log, 0, SHADER_ERR_LOG_SIZE);
    _err_log_idx = 0;
}

Shader::~Shader() {
    glDeleteShader(_vs);
    glDeleteShader(_fs);
    glDeleteProgram(_id);
}

void Shader::compile(std::string fsSrc) {
    glDeleteShader(_vs);
    glDeleteShader(_fs);
    glDeleteProgram(_id);

    _uniforms.clear();

    _error = false;
    memset(_err_log, 0, SHADER_ERR_LOG_SIZE);
    _err_log_idx = 0;

    int vs = compile_shader(GL_VERTEX_SHADER, vsSrc);
    int fs = compile_shader(GL_FRAGMENT_SHADER, fsSrc);

    create_program(vs, fs);
}

void Shader::create_program(GLuint vs, GLuint fs) {
    _id = glCreateProgram();

    glAttachShader(_id, vs);
    glAttachShader(_id, fs);
    glLinkProgram(_id);

    _vs = vs;
    _fs = fs;

    bind();
}

int Shader::compile_shader(GLenum type, const std::string &source) {
    int id = glCreateShader(type);
    const char *c_str = source.c_str();
    glShaderSource(id, 1, &c_str, nullptr);
    glCompileShader(id);

    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);

    if (status != 1) {
        sprintf(&_err_log[_err_log_idx], "[SHADER COMPILE] %s  Status %d\n\n", ((type == GL_VERTEX_SHADER) ? "VertexShader" : "FragmentShader"), status);
        _err_log_idx = (int)strlen(_err_log);

        printf("%s\n", _err_log);

        GLsizei logLength = 0;
        glGetShaderInfoLog(id, 1024, &logLength, &_err_log[_err_log_idx]);
        _err_log_idx += logLength;

        _error = true;
    }

    return id;
}

void Shader::bind() const {
    glUseProgram(_id);
}

void Shader::unbind() {
    glUseProgram(0);
}

int Shader::uniform_location(const std::string &name) {
    if (_uniforms.count(name))
        return _uniforms[name];

    GLint loc = glGetUniformLocation(_id, name.c_str());
    _uniforms[name] = loc;
    if (loc == -1)
        printf("[ERROR] Uniform %s not found\n", name.c_str());

    return loc;
}

void Shader::uniform1li(const std::string &name, int x) {
    GLint loc = uniform_location(name);
    if (loc != -1)
        glUniform1i(loc, x);
}

void Shader::uniform1lf(const std::string &name, float x) {
    GLint loc = uniform_location(name);
    if (loc != -1)
        glUniform1f(loc, x);
}

void Shader::uniformM4fv(const std::string &name, glm::mat4x4 mvp) {
    GLint loc = uniform_location(name);
    if (loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]);
}

void Shader::uniform2fv(const std::string &name, glm::vec2 v) {
    GLint loc = uniform_location(name);
    if (loc != -1)
        glUniform2fv(loc, 1, &v[0]);
}

void Shader::uniform3fv(const std::string &name, glm::vec3 v) {
    GLint loc = uniform_location(name);
    if (loc != -1)
        glUniform3fv(loc, 1, &v[0]);
}

void Shader::uniform4fv(const std::string &name, glm::vec4 v) {
    GLint loc = uniform_location(name);
    if (loc != -1)
        glUniform4fv(loc, 1, &v[0]);
}

void Shader::uniform1iv(const std::string &name, GLsizei count, const GLint *value) {
    GLint loc = uniform_location(name);
    if (loc != -1)
        glUniform1iv(loc, count, value);
}

bool Shader::error() {
    return _error;
}

char *Shader::getLog() {
    return _err_log;
}

void Shader::build(Project *project) {
    std::stringstream ss;

    for(auto* file : project->getFiles()){
        std::string line;
        std::ifstream source(file->path.c_str());
        while (std::getline(source, line))
            ss << line << std::endl;
        source.close();
    }

    compile(ss.str());
}

