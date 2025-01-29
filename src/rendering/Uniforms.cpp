#include "rendering/Uniforms.h"
#include "config/Project.h"

void Uniforms::update(GLFWwindow* window) {

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
    _tt += _dt;

    dt = (float)_dt;
    tt = (float)_tt;
}
