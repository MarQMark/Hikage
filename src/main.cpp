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
#include "script/Callbacks.h"
#include "rendering/Icon.h"

/* TODO:
 *  - Add Info
 *  - Info Uniforms
 *  - Info Python Scripts
 *
 *  - Add Script Error Popup
 *  - Add periodic reload
 *  - Settings Menu
 *  - Fix Version compatibility
 *  - Single Frame Generation
 *  - Multiple Shader Source files
 *  - Vertex Shader
 *  - Different Aspect Ratio to Resolution
 *  - Preprocessor in shader
 *  - Better Error Log (show error in code)
 *  - (Add deb package)
 */

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

    Icon icon;
    icon.gen(window, "Hikage");

    Shader shader;
    Viewport viewport(1280, 720);
    viewport.setShader(&shader);
    Uniforms::get()->window = window;

    scriptAddCallbacks(window);

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
            Uniforms::get()->update();

            startMenu.setVisible(false);

            if(Project::opened()){
                glfwSetWindowTitle(window, Project::get()->getName().c_str());
                viewport.update(Project::get()->getWidth(), Project::get()->getHeight());
                icon.gen(window, Project::get()->getName());
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

                if(Project::get()->getScript()){

                    Project::get()->getScript()->update();
                }

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
