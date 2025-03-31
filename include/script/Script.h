#ifndef HIKAGE_SCRIPT_H
#define HIKAGE_SCRIPT_H


#include <string>
#include <Python.h>
#include <vector>
#include "glm/glm.hpp"

class Script {
public:
    explicit Script(std::string path);

    std::string getPath();

    void launch();
    void update();

    void setActive(bool active);
    bool isActive() const;

    void clbkMouseBtn(int btn, int action, int mods);
    void clbkMousePos(double x, double y);
    void clbkKey(int key, int scancode, int action, int mods);

    enum UniformType{
        i1 = 0,
        f1,
        f2,
        f3,
        f4
    };

    union UniformValue{
        int i1;
        float f1;
        glm::vec2 f2;
        glm::vec3 f3;
        glm::vec4 f4;
    };
    struct Uniform{
        std::string name;
        enum UniformType type;
        union UniformValue value;
    };
    std::vector<Script::Uniform>* getUniforms();
    void addUniform(const Script::Uniform& uniform);

private:
    static bool _init;

    std::string _path;

    bool _valid = false;
    bool _active = false;

    std::vector<Uniform> _uniforms;

    PyObject* _py_module{};
    PyObject* _py_init{};
    PyObject* _py_update{};
    PyObject* _py_clbk_mouse_btn{};
    PyObject* _py_clbk_mouse_pos{};
    PyObject* _py_clbk_key{};

    void py_call_func(PyObject* func, PyObject* args) const;
};


#endif //HIKAGE_SCRIPT_H
