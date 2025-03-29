#include "script/Callbacks.h"
#include "config/Project.h"

void clbk_mouse_btn(GLFWwindow* w, int btn, int action, int mods){
    if(Project::get() && Project::get()->getScript())
        Project::get()->getScript()->clbkMouseBtn(btn, action, mods);
}

void clbk_mouse_pos(GLFWwindow* w, double x, double y){
    if(Project::get() && Project::get()->getScript())
        Project::get()->getScript()->clbkMousePos(x, y);
}

void clbk_key_callback(GLFWwindow* w, int key, int scancode, int action, int mods){
    if(Project::get() && Project::get()->getScript())
        Project::get()->getScript()->clbkKey(key, scancode, action, mods);
}

void scriptAddCallbacks(GLFWwindow* window){
    glfwSetMouseButtonCallback(window, clbk_mouse_btn);
    glfwSetCursorPosCallback(window, clbk_mouse_pos);
    glfwSetKeyCallback(window, clbk_key_callback);
}