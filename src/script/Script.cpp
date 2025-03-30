#include "script/Script.h"

#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "config/Config.h"
#include "config/Project.h"
#include "rendering/Uniforms.h"

/*  Functions:
 *      - void pySetUniform1i(name, int)
 *      - void pySetUniform1f(name, float)
 *      - void pySetUniform2f(name, float1, float2)
 *      - void pySetUniform3g(name, float1, float2, float3)
 *      - void pySetUniform4f(name, float1, float2, float3, float 4)
 *      - void pySetCursorPos(x, y)
 *      - void pySetCursorVisibility(visible)
 *      - double pyGetDT()
 *      - double pyGetTT()
 *      - float pyGetResolutionX()
 *      - float pyGetResolutionY()
 *      - float pyGetWindowX()
 *      - float pyGetWindowY()
 *
 *  Callbacks:
 *      - void init()
 *      - void update()
 *      - void clbkMouseBtn(btn, action, mods)
 *      - void clbkMousePos(x, y)
 *      - void clbkKey(key, scancode, action, mods)
 */

static PyObject* pySetUniform1i(PyObject* self, PyObject* args) {
    const char* input_str;
    int int1;

    if (!PyArg_ParseTuple(args, "si", &input_str, &int1))
        return nullptr;

    Script::Uniform u;
    u.name = std::string(input_str);
    u.type = Script::UniformType::i1;
    u.value.i1 = int1;
    Project::get()->getScript()->addUniform(u);
    Py_RETURN_NONE;
}
static PyObject* pySetUniform1f(PyObject* self, PyObject* args) {
    const char* input_str;
    double float1;

    if (!PyArg_ParseTuple(args, "sd", &input_str, &float1))
        return nullptr;

    Script::Uniform u;
    u.name = std::string(input_str);
    u.type = Script::UniformType::f1;
    u.value.f1 = (float)float1;
    Project::get()->getScript()->addUniform(u);
    Py_RETURN_NONE;
}
static PyObject* pySetUniform2f(PyObject* self, PyObject* args) {
    const char* input_str;
    double float1, float2;

    if (!PyArg_ParseTuple(args, "sdd", &input_str, &float1, &float2))
        return nullptr;

    Script::Uniform u;
    u.name = std::string(input_str);
    u.type = Script::UniformType::f2;
    u.value.f2 = glm::vec2(float1, float2);
    Project::get()->getScript()->addUniform(u);
    Py_RETURN_NONE;
}
static PyObject* pySetUniform3f(PyObject* self, PyObject* args) {
    const char* input_str;
    double float1, float2, float3;

    if (!PyArg_ParseTuple(args, "sddd", &input_str, &float1, &float2, &float3))
        return nullptr;

    Script::Uniform u;
    u.name = std::string(input_str);
    u.type = Script::UniformType::f3;
    u.value.f3 = glm::vec3(float1, float2, float3);
    Project::get()->getScript()->addUniform(u);
    Py_RETURN_NONE;
}
static PyObject* pySetUniform4f(PyObject* self, PyObject* args) {
    const char* input_str;
    double float1, float2, float3, float4;

    if (!PyArg_ParseTuple(args, "sddd", &input_str, &float1, &float2, &float3, &float4))
        return nullptr;

    Script::Uniform u;
    u.name = std::string(input_str);
    u.type = Script::UniformType::f4;
    u.value.f4 = glm::vec4(float1, float2, float3, float4);
    Project::get()->getScript()->addUniform(u);
    Py_RETURN_NONE;
}

static PyObject* pySetCursorPos(PyObject* self, PyObject* args){
    double x, y;

    if (!PyArg_ParseTuple(args, "dd", &x, &y))
        return nullptr;

    Uniforms::get()->setCursorPos = true;
    Uniforms::get()->cursorPos = glm::vec2(x, y);
    Py_RETURN_NONE;
}
static PyObject* pySetCursorVisibility(PyObject* self, PyObject* args) {
    PyObject* pyBool;

    if (!PyArg_ParseTuple(args, "O", &pyBool)) {
        return nullptr;
    }
    if (!PyBool_Check(pyBool)) {
        PyErr_SetString(PyExc_TypeError, "Expected a boolean value");
        return nullptr;
    }

    bool cppBool = PyObject_IsTrue(pyBool);
    Uniforms::get()->updateCursorVis = true;
    Uniforms::get()->cursorVisible = cppBool;
    Py_RETURN_NONE;
}

static PyObject* pyGetDT(PyObject* self, PyObject* args) {
    return PyFloat_FromDouble(Uniforms::get()->dt);
}
static PyObject* pyGetTT(PyObject* self, PyObject* args) {
    return PyFloat_FromDouble(Uniforms::get()->tt);
}
static PyObject* pyGetResolutionX(PyObject* self, PyObject* args) {
    return PyFloat_FromDouble(Uniforms::get()->dim.x);
}
static PyObject* pyGetResolutionY(PyObject* self, PyObject* args) {
    return PyFloat_FromDouble(Uniforms::get()->dim.y);
}
static PyObject* pyGetWindowW(PyObject* self, PyObject* args) {
    return PyFloat_FromDouble(Uniforms::get()->windowDim.x);
}
static PyObject* pyGetWindowH(PyObject* self, PyObject* args) {
    return PyFloat_FromDouble(Uniforms::get()->windowDim.y);
}

static PyMethodDef ScriptMethods[] = {
        {"setUniform1i", pySetUniform1i, METH_VARARGS, "Set int uniform"},
        {"setUniform1f", pySetUniform1f, METH_VARARGS, "Set float uniform"},
        {"setUniform2f", pySetUniform2f, METH_VARARGS, "Set vec2 uniform"},
        {"setUniform3f", pySetUniform3f, METH_VARARGS, "Set vec3 uniform"},
        {"setUniform4f", pySetUniform4f, METH_VARARGS, "Set vec4 uniform"},
        {"setCursorPos", pySetCursorPos, METH_VARARGS, "Set position of mouse cursor"},
        {"setCursorVisibility", pySetCursorVisibility, METH_VARARGS, "Change visibility of mouse cursor"},
        {"getDT", pyGetDT, METH_NOARGS, "Returns delta time of last frame in ms"},
        {"getTT", pyGetTT, METH_NOARGS, "Returns total time since last start in ms"},
        {"getResolutionX", pyGetResolutionX, METH_NOARGS, "Returns X resolution of render texture"},
        {"getResolutionY", pyGetResolutionY, METH_NOARGS, "Returns Y resolution of render texture"},
        {"getWindowW", pyGetWindowW, METH_NOARGS, "Returns width of window"},
        {"getWindowH", pyGetWindowH, METH_NOARGS, "Returns height of window"},
        {nullptr, nullptr, 0, nullptr}
};
static PyModuleDef ScriptModule = {
        PyModuleDef_HEAD_INIT, "hikage", nullptr, -1, ScriptMethods
};

void addConstants(PyObject* module){
    PyModule_AddIntConstant(module, "KEY_PRESS", GLFW_PRESS);
    PyModule_AddIntConstant(module, "KEY_RELEASE", GLFW_RELEASE);
    PyModule_AddIntConstant(module, "KEY_REPEAT", GLFW_REPEAT);

    PyModule_AddIntConstant(module, "KEY_SPACE", GLFW_KEY_SPACE);
    PyModule_AddIntConstant(module, "KEY_APOSTROPHE", GLFW_KEY_APOSTROPHE);
    PyModule_AddIntConstant(module, "KEY_COMMA", GLFW_KEY_COMMA);
    PyModule_AddIntConstant(module, "KEY_MINUS", GLFW_KEY_MINUS);
    PyModule_AddIntConstant(module, "KEY_PERIOD", GLFW_KEY_PERIOD);
    PyModule_AddIntConstant(module, "KEY_SLASH", GLFW_KEY_SLASH);
    PyModule_AddIntConstant(module, "KEY_0", GLFW_KEY_0);
    PyModule_AddIntConstant(module, "KEY_1", GLFW_KEY_1);
    PyModule_AddIntConstant(module, "KEY_2", GLFW_KEY_2);
    PyModule_AddIntConstant(module, "KEY_3", GLFW_KEY_3);
    PyModule_AddIntConstant(module, "KEY_4", GLFW_KEY_4);
    PyModule_AddIntConstant(module, "KEY_5", GLFW_KEY_5);
    PyModule_AddIntConstant(module, "KEY_6", GLFW_KEY_6);
    PyModule_AddIntConstant(module, "KEY_7", GLFW_KEY_7);
    PyModule_AddIntConstant(module, "KEY_8", GLFW_KEY_8);
    PyModule_AddIntConstant(module, "KEY_9", GLFW_KEY_9);
    PyModule_AddIntConstant(module, "KEY_SEMICOLON", GLFW_KEY_SEMICOLON);
    PyModule_AddIntConstant(module, "KEY_EQUAL", GLFW_KEY_EQUAL);
    PyModule_AddIntConstant(module, "KEY_A", GLFW_KEY_A);
    PyModule_AddIntConstant(module, "KEY_B", GLFW_KEY_B);
    PyModule_AddIntConstant(module, "KEY_C", GLFW_KEY_C);
    PyModule_AddIntConstant(module, "KEY_D", GLFW_KEY_D);
    PyModule_AddIntConstant(module, "KEY_E", GLFW_KEY_E);
    PyModule_AddIntConstant(module, "KEY_F", GLFW_KEY_F);
    PyModule_AddIntConstant(module, "KEY_G", GLFW_KEY_G);
    PyModule_AddIntConstant(module, "KEY_H", GLFW_KEY_H);
    PyModule_AddIntConstant(module, "KEY_I", GLFW_KEY_I);
    PyModule_AddIntConstant(module, "KEY_J", GLFW_KEY_J);
    PyModule_AddIntConstant(module, "KEY_K", GLFW_KEY_K);
    PyModule_AddIntConstant(module, "KEY_L", GLFW_KEY_L);
    PyModule_AddIntConstant(module, "KEY_M", GLFW_KEY_M);
    PyModule_AddIntConstant(module, "KEY_N", GLFW_KEY_N);
    PyModule_AddIntConstant(module, "KEY_O", GLFW_KEY_O);
    PyModule_AddIntConstant(module, "KEY_P", GLFW_KEY_P);
    PyModule_AddIntConstant(module, "KEY_Q", GLFW_KEY_Q);
    PyModule_AddIntConstant(module, "KEY_R", GLFW_KEY_R);
    PyModule_AddIntConstant(module, "KEY_S", GLFW_KEY_S);
    PyModule_AddIntConstant(module, "KEY_T", GLFW_KEY_T);
    PyModule_AddIntConstant(module, "KEY_U", GLFW_KEY_U);
    PyModule_AddIntConstant(module, "KEY_V", GLFW_KEY_V);
    PyModule_AddIntConstant(module, "KEY_W", GLFW_KEY_W);
    PyModule_AddIntConstant(module, "KEY_X", GLFW_KEY_X);
    PyModule_AddIntConstant(module, "KEY_Y", GLFW_KEY_Y);
    PyModule_AddIntConstant(module, "KEY_Z", GLFW_KEY_Z);
    PyModule_AddIntConstant(module, "KEY_LEFT_BRACKET", GLFW_KEY_LEFT_BRACKET);
    PyModule_AddIntConstant(module, "KEY_BACKSLASH", GLFW_KEY_BACKSLASH);
    PyModule_AddIntConstant(module, "KEY_RIGHT_BRACKET", GLFW_KEY_RIGHT_BRACKET);
    PyModule_AddIntConstant(module, "KEY_GRAVE_ACCENT", GLFW_KEY_GRAVE_ACCENT);
    PyModule_AddIntConstant(module, "KEY_WORLD_1", GLFW_KEY_WORLD_1);
    PyModule_AddIntConstant(module, "KEY_WORLD_2", GLFW_KEY_WORLD_2);

    PyModule_AddIntConstant(module, "KEY_ESCAPE", GLFW_KEY_ESCAPE);
    PyModule_AddIntConstant(module, "KEY_ENTER", GLFW_KEY_ENTER);
    PyModule_AddIntConstant(module, "KEY_TAB", GLFW_KEY_TAB);
    PyModule_AddIntConstant(module, "KEY_BACKSPACE", GLFW_KEY_BACKSPACE);
    PyModule_AddIntConstant(module, "KEY_INSERT", GLFW_KEY_INSERT);
    PyModule_AddIntConstant(module, "KEY_DELETE", GLFW_KEY_DELETE);
    PyModule_AddIntConstant(module, "KEY_RIGHT", GLFW_KEY_RIGHT);
    PyModule_AddIntConstant(module, "KEY_LEFT", GLFW_KEY_LEFT);
    PyModule_AddIntConstant(module, "KEY_DOWN", GLFW_KEY_DOWN);
    PyModule_AddIntConstant(module, "KEY_UP", GLFW_KEY_UP);
    PyModule_AddIntConstant(module, "KEY_PAGE_UP", GLFW_KEY_PAGE_UP);
    PyModule_AddIntConstant(module, "KEY_PAGE_DOWN", GLFW_KEY_PAGE_DOWN);
    PyModule_AddIntConstant(module, "KEY_HOME", GLFW_KEY_HOME);
    PyModule_AddIntConstant(module, "KEY_END", GLFW_KEY_END);
    PyModule_AddIntConstant(module, "KEY_CAPS_LOCK", GLFW_KEY_CAPS_LOCK);
    PyModule_AddIntConstant(module, "KEY_SCROLL_LOCK", GLFW_KEY_SCROLL_LOCK);
    PyModule_AddIntConstant(module, "KEY_NUM_LOCK", GLFW_KEY_NUM_LOCK);
    PyModule_AddIntConstant(module, "KEY_PRINT_SCREEN", GLFW_KEY_PRINT_SCREEN);
    PyModule_AddIntConstant(module, "KEY_PAUSE", GLFW_KEY_PAUSE);
    PyModule_AddIntConstant(module, "KEY_F1", GLFW_KEY_F1);
    PyModule_AddIntConstant(module, "KEY_F2", GLFW_KEY_F2);
    PyModule_AddIntConstant(module, "KEY_F3", GLFW_KEY_F3);
    PyModule_AddIntConstant(module, "KEY_F4", GLFW_KEY_F4);
    PyModule_AddIntConstant(module, "KEY_F5", GLFW_KEY_F5);
    PyModule_AddIntConstant(module, "KEY_F6", GLFW_KEY_F6);
    PyModule_AddIntConstant(module, "KEY_F7", GLFW_KEY_F7);
    PyModule_AddIntConstant(module, "KEY_F8", GLFW_KEY_F8);
    PyModule_AddIntConstant(module, "KEY_F9", GLFW_KEY_F9);
    PyModule_AddIntConstant(module, "KEY_F10", GLFW_KEY_F10);
    PyModule_AddIntConstant(module, "KEY_F11", GLFW_KEY_F11);
    PyModule_AddIntConstant(module, "KEY_F12", GLFW_KEY_F12);
    PyModule_AddIntConstant(module, "KEY_F13", GLFW_KEY_F13);
    PyModule_AddIntConstant(module, "KEY_F14", GLFW_KEY_F14);
    PyModule_AddIntConstant(module, "KEY_F15", GLFW_KEY_F15);
    PyModule_AddIntConstant(module, "KEY_F16", GLFW_KEY_F16);
    PyModule_AddIntConstant(module, "KEY_F17", GLFW_KEY_F17);
    PyModule_AddIntConstant(module, "KEY_F18", GLFW_KEY_F18);
    PyModule_AddIntConstant(module, "KEY_F19", GLFW_KEY_F19);
    PyModule_AddIntConstant(module, "KEY_F20", GLFW_KEY_F20);
    PyModule_AddIntConstant(module, "KEY_F21", GLFW_KEY_F21);
    PyModule_AddIntConstant(module, "KEY_F22", GLFW_KEY_F22);
    PyModule_AddIntConstant(module, "KEY_F23", GLFW_KEY_F23);
    PyModule_AddIntConstant(module, "KEY_F24", GLFW_KEY_F24);
    PyModule_AddIntConstant(module, "KEY_F25", GLFW_KEY_F25);
    PyModule_AddIntConstant(module, "KEY_KP_0", GLFW_KEY_KP_0);
    PyModule_AddIntConstant(module, "KEY_KP_1", GLFW_KEY_KP_1);
    PyModule_AddIntConstant(module, "KEY_KP_2", GLFW_KEY_KP_2);
    PyModule_AddIntConstant(module, "KEY_KP_3", GLFW_KEY_KP_3);
    PyModule_AddIntConstant(module, "KEY_KP_4", GLFW_KEY_KP_4);
    PyModule_AddIntConstant(module, "KEY_KP_5", GLFW_KEY_KP_5);
    PyModule_AddIntConstant(module, "KEY_KP_6", GLFW_KEY_KP_6);
    PyModule_AddIntConstant(module, "KEY_KP_7", GLFW_KEY_KP_7);
    PyModule_AddIntConstant(module, "KEY_KP_8", GLFW_KEY_KP_8);
    PyModule_AddIntConstant(module, "KEY_KP_9", GLFW_KEY_KP_9);
    PyModule_AddIntConstant(module, "KEY_KP_DECIMAL", GLFW_KEY_KP_DECIMAL);
    PyModule_AddIntConstant(module, "KEY_KP_DIVIDE", GLFW_KEY_KP_DIVIDE);
    PyModule_AddIntConstant(module, "KEY_KP_MULTIPLY", GLFW_KEY_KP_MULTIPLY);
    PyModule_AddIntConstant(module, "KEY_KP_SUBTRACT", GLFW_KEY_KP_SUBTRACT);
    PyModule_AddIntConstant(module, "KEY_KP_ADD", GLFW_KEY_KP_ADD);
    PyModule_AddIntConstant(module, "KEY_KP_ENTER", GLFW_KEY_KP_ENTER);
    PyModule_AddIntConstant(module, "KEY_KP_EQUAL", GLFW_KEY_KP_EQUAL);
    PyModule_AddIntConstant(module, "KEY_LEFT_SHIFT", GLFW_KEY_LEFT_SHIFT);
    PyModule_AddIntConstant(module, "KEY_LEFT_CONTROL", GLFW_KEY_LEFT_CONTROL);
    PyModule_AddIntConstant(module, "KEY_LEFT_ALT", GLFW_KEY_LEFT_ALT);
    PyModule_AddIntConstant(module, "KEY_LEFT_SUPER", GLFW_KEY_LEFT_SUPER);
    PyModule_AddIntConstant(module, "KEY_RIGHT_SHIFT", GLFW_KEY_RIGHT_SHIFT);
    PyModule_AddIntConstant(module, "KEY_RIGHT_CONTROL", GLFW_KEY_RIGHT_CONTROL);
    PyModule_AddIntConstant(module, "KEY_RIGHT_ALT", GLFW_KEY_RIGHT_ALT);
    PyModule_AddIntConstant(module, "KEY_RIGHT_SUPER", GLFW_KEY_RIGHT_SUPER);
    PyModule_AddIntConstant(module, "KEY_MENU", GLFW_KEY_MENU);

    PyModule_AddIntConstant(module, "MOUSE_BUTTON_1", GLFW_MOUSE_BUTTON_1);
    PyModule_AddIntConstant(module, "MOUSE_BUTTON_2", GLFW_MOUSE_BUTTON_2);
    PyModule_AddIntConstant(module, "MOUSE_BUTTON_3", GLFW_MOUSE_BUTTON_3);
    PyModule_AddIntConstant(module, "MOUSE_BUTTON_4", GLFW_MOUSE_BUTTON_4);
    PyModule_AddIntConstant(module, "MOUSE_BUTTON_5", GLFW_MOUSE_BUTTON_5);
    PyModule_AddIntConstant(module, "MOUSE_BUTTON_6", GLFW_MOUSE_BUTTON_6);
    PyModule_AddIntConstant(module, "MOUSE_BUTTON_7", GLFW_MOUSE_BUTTON_7);
    PyModule_AddIntConstant(module, "MOUSE_BUTTON_8", GLFW_MOUSE_BUTTON_8);
    PyModule_AddIntConstant(module, "MOUSE_BUTTON_LAST", GLFW_MOUSE_BUTTON_LAST);
    PyModule_AddIntConstant(module, "MOUSE_BUTTON_LEFT", GLFW_MOUSE_BUTTON_LEFT);
    PyModule_AddIntConstant(module, "MOUSE_BUTTON_RIGHT", GLFW_MOUSE_BUTTON_RIGHT);
    PyModule_AddIntConstant(module, "MOUSE_BUTTON_MIDDLE", GLFW_MOUSE_BUTTON_MIDDLE);
}

PyMODINIT_FUNC pyInitScriptAPI() {
    PyObject* module = PyModule_Create(&ScriptModule);
    addConstants(module);
    return module;
}



Script::Script(std::string path) : _path(std::move(path)) {
}

std::string Script::getPath() {
    return _path + PROJECT_SCRIPT_NAME;
}

void addFunc(PyObject* module, PyObject** func, const std::string& name){
    if(PyObject_HasAttrString(module, name.c_str())){
        *func = PyObject_GetAttrString(module, name.c_str());
        if (!*func || !PyCallable_Check(*func)) {
            *func = nullptr;
        }
    }
}

void Script::launch() {
    if(_valid){
        Py_DECREF(_py_module);
        Py_Finalize();

        _py_init = nullptr;
        _py_update = nullptr;
        _py_clbk_mouse_btn = nullptr;
        _py_clbk_mouse_pos = nullptr;
        _py_clbk_key = nullptr;
    }

    _uniforms.clear();
    PyImport_AppendInittab("hikage", &pyInitScriptAPI);
    Py_Initialize();
    PyRun_SimpleString(std::string("import sys; sys.path.append('"+ _path +"')").c_str());

    _active = false;
    _valid = false;


    _py_module = PyImport_ImportModule("script");
    if (!_py_module) {
        PyErr_Print();
        printf("Failed to load script.py\n");
        Py_Finalize();
        return;
    }

    addFunc(_py_module, &_py_init, "init");
    addFunc(_py_module, &_py_update, "update");
    addFunc(_py_module, &_py_clbk_mouse_btn, "clbkMouseBtn");
    addFunc(_py_module, &_py_clbk_mouse_pos, "clbkMousePos");
    addFunc(_py_module, &_py_clbk_key, "clbkKey");

    if(_py_init)
        PyObject_CallObject(_py_init, nullptr);

    _active = true;
    _valid = true;
}

void Script::setActive(bool active) {
    if(!_valid || active){
        launch();
        _active = _valid;
    }
    else
        _active = active;
}

bool Script::isActive() const {
    return _active;
}

void Script::py_call_func(PyObject* func, PyObject* args = nullptr) const{
    if(!_active)
        return;

    if(func)
        PyObject_CallObject(func, args);
}

void Script::update() {
    py_call_func(_py_update);
}

void Script::clbkMouseBtn(int btn, int action, int mods) {
    if(!_valid) return;
    PyObject* args = PyTuple_Pack(3, PyLong_FromLong(btn), PyLong_FromLong(action), PyLong_FromLong(mods));
    py_call_func(_py_clbk_mouse_btn, args);
}

void Script::clbkMousePos(double x, double y) {
    if(!_valid) return;
    PyObject* args = PyTuple_Pack(2, PyFloat_FromDouble(x), PyFloat_FromDouble(y));
    py_call_func(_py_clbk_mouse_pos, args);
}

void Script::clbkKey(int key, int scancode, int action, int mods) {
    if(!_valid) return;
    PyObject* args = PyTuple_Pack(4, PyLong_FromLong(key), PyLong_FromLong(scancode), PyLong_FromLong(action), PyLong_FromLong(mods));
    py_call_func(_py_clbk_key, args);
}

std::vector<Script::Uniform> *Script::getUniforms() {
    return &_uniforms;
}

void Script::addUniform(const Script::Uniform& uniform) {
    for(auto& u : _uniforms){
        if(u.name == uniform.name){
            u.type = uniform.type;
            u.value = uniform.value;
            return;
        }
    }

    _uniforms.push_back(uniform);
}

