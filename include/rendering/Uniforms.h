#ifndef HIKAGE_UNIFORMS_H
#define HIKAGE_UNIFORMS_H

#include <chrono>
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

class Uniforms {
public:
    Uniforms() = default;

    void update(GLFWwindow* window);

    float dt;
    float tt;
    glm::vec2 dim;
    glm::vec2 mousePos;

private:
    double _dt = 0;
    double _tt = 0;

    std::chrono::high_resolution_clock::time_point _last_time;
};


#endif //HIKAGE_UNIFORMS_H
