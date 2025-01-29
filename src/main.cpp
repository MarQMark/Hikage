#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "config/Config.h"
#include "config/Project.h"
#include "views/View.h"
#include "views/Menubar.h"
#include "views/ShaderView.h"
#include "views/NewProject.h"

int main() {

    if(Config::open() == -1){
        return -1;
    }

    Project::open(CONFIG_PATH "test.frag");

    if (!glfwInit()){
        printf("[ERROR] Could not initialize GLFW\n");
        return -1;
    }

    auto* window = glfwCreateWindow(1280, 720, "Hikage", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        printf("[ERROR] Could create Window\n");
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK){
        printf("[ERROR] Could not initialize GLEW\n");
        return -1;
    }

    View view(window);
    Menubar menubar;
    ShaderView shaderView;
    NewProject newProject;

    view.addViewable(&menubar, "Menubar");
    view.addViewable(&shaderView, "ShaderView");
    view.addViewable(&newProject, "NewProject");

    while(!glfwWindowShouldClose(window)){
        view.render();
    }

    return 0;
}
