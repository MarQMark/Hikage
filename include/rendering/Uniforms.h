#ifndef HIKAGE_UNIFORMS_H
#define HIKAGE_UNIFORMS_H

#include <chrono>
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

class Uniforms {
private:
    static Uniforms* s_instance;

public:
    static Uniforms* get(){
        if(!s_instance)
            s_instance = new Uniforms;

        return s_instance;
    }

    void update();

    float dt;
    float tt;
    glm::vec2 dim{};
    glm::vec2 mousePos{};
    GLFWwindow* window{};

    glm::vec2 windowDim{};

    bool setCursorPos = false;
    glm::vec2 cursorPos{};
    bool updateCursorVis = false;
    bool cursorVisible = true;

private:
    Uniforms() {
        dt = 0;
        tt = 0;
        dim = glm::vec2(1280, 720);
        mousePos = glm::vec2(0);
    }

    double _dt = 0;
    double _tt = 0;

    std::chrono::high_resolution_clock::time_point _last_time;
};


#endif //HIKAGE_UNIFORMS_H
