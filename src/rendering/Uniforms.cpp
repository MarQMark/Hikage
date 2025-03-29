#include "rendering/Uniforms.h"
#include "config/Project.h"

Uniforms* Uniforms::s_instance = nullptr;

void Uniforms::update() {

    double mouseX;
    double mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    mousePos.x = (float)mouseX;
    mousePos.y = (float)mouseY;

    dim.x = (float)Project::get()->getWidth();
    dim.y = (float)Project::get()->getHeight();

    _dt = ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - _last_time)).count();
    _last_time = std::chrono::high_resolution_clock::now();
    if(_dt > 1e7)
        _dt = 1e-7;
    if(Project::get() && Project::get()->isPaused())
        return;

    _tt += _dt;

    dt = (float)_dt;
    tt = (float)_tt;

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    windowDim = glm::vec2(width, height);

    if(setCursorPos){
        glfwSetCursorPos(window, cursorPos.x, cursorPos.y);
        setCursorPos = false;
    }
    if(updateCursorVis){
        if(cursorVisible)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        updateCursorVis = false;
    }
}
