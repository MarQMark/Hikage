#ifndef HIKAGE_VIEW_H
#define HIKAGE_VIEW_H


#include <map>
#include <string>
#include "GLFW/glfw3.h"
#include "Viewable.h"

class View {
public:
    View(GLFWwindow* window);

    void render();
    void addViewable(Viewable* viewable, const std::string& name);
    Viewable* getViewable(const std::string& name);

private:
    GLFWwindow* _window;

    std::map<std::string, Viewable*> _viewables;
};


#endif //HIKAGE_VIEW_H
