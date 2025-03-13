#include <fstream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "config/Config.h"
#include "config/Project.h"
#include "views/View.h"
#include "views/Menubar.h"
#include "views/ShaderView.h"
#include "views/NewProject.h"
#include "rendering/Shader.h"
#include "views/ErrorLog.h"
#include "views/StartMenu.h"
#include "views/ViewportSettings.h"
#include "views/TexturesView.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image/stb_image.h"
#include "rendering/Uniforms.h"


/* TODO:
 *  - Add pause/ play
 *  - Add periodic reload
 *  - Fix Version compatibility
 *  - Add Info
 *  - (Add deb package)
 *
 *  - Single Frame Generation
 *  - Stats (FPS, Mouse Pos, ...)
 *  - Multiple Shader Source files
 *  - Vertex Shader
 *  - Scripting
 *  - Different Aspect Ratio to Resolution
 *  - Info Uniforms
 *  - Preprocessor in shader
 *  - Better Error Log (show error in code)
 */

void setWindowIcon(GLFWwindow* window, const char* iconPath) {
    GLFWimage icon;
    int width, height, channels;

    unsigned char* imageData = stbi_load(iconPath, &width, &height, &channels, 4);
    if (!imageData) {
        printf("Failed to load icon image: %s\n", iconPath);
        return;
    }

    icon.width = width;
    icon.height = height;
    icon.pixels = imageData;

    glfwSetWindowIcon(window, 1, &icon);

    stbi_image_free(imageData);
}

int main() {

    if(Config::open() == -1){
        return -1;
    }

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

    //setWindowIcon(window, std::string(Config::get()->configPath() + "/icon.png").c_str());

    Shader shader;
    Uniforms uniforms;
    Viewport viewport(1280, 720);
    viewport.setShader(&shader);
    viewport.setUniforms(&uniforms);

    View view(window);
    Menubar menubar;
    ShaderView shaderView;
    NewProject newProject;
    ErrorLog errorLog;
    StartMenu startMenu;
    ViewportSettings viewportSettings;
    TexturesView texturesView;

    view.addViewable(&menubar, "Menubar");
    view.addViewable(&shaderView, "ShaderView");
    view.addViewable(&newProject, "NewProject");
    view.addViewable(&errorLog, "ErrorLog");
    view.addViewable(&startMenu, "StartMenu");
    view.addViewable(&viewportSettings, "ViewportSettings");
    view.addViewable(&texturesView, "TexturesView");

    startMenu.setVisible(true);
    errorLog.setVisible(false);
    shaderView.setVisible(false);
    shaderView.setViewport(&viewport);
    viewportSettings.setViewport(&viewport);

    while(!glfwWindowShouldClose(window)){
        view.render();

        if(Project::get()){
            uniforms.update(window);

            startMenu.setVisible(false);

            if(Project::opened()){
                glfwSetWindowTitle(window, Project::get()->getName().c_str());
                viewport.update(Project::get()->getWidth(), Project::get()->getHeight());
            }

            if(Project::get()->changed()){
                shader.build(Project::get());
            }

            if(shader.error()){
                shaderView.setVisible(false);
                errorLog.setVisible(true);

                errorLog.setLog(shader.getLog());
            }
            else{
                shaderView.setVisible(true);
                errorLog.setVisible(false);

                shader.bind();
                viewport.render();
            }
        }
        else{
            startMenu.setVisible(true);
            errorLog.setVisible(false);
            shaderView.setVisible(false);
        }
    }

    return 0;
}
