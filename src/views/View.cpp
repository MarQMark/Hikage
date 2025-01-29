#include "views/View.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

View::View(GLFWwindow *window) : _window(window) {
    // Setup ImGUI
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

void View::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto &viewable: _viewables) {
        if(viewable.second->isVisible())
            viewable.second->render();
    }

    //bool show_demo_window = true;
    //ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void View::addViewable(Viewable *viewable, const std::string& name) {
    _viewables[name] = viewable;
    viewable->view = this;
}

Viewable *View::getViewable(const std::string &name) {
    if(_viewables.count(name))
        return _viewables[name];

    printf("[ERROR] Viewable %s not found\n", name.c_str());
    return nullptr;
}
