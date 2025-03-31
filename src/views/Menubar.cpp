#include "views/Menubar.h"
#include "imgui/imgui.h"
#include "views/View.h"
#include "config/Config.h"
#include "util/utilOS.h"

void Menubar::open_project(Project* project){
    if(project->load() == 1){
        Project::open(project);
    }
    else{
        printf("[ERROR] opening Project\n");
        delete project;
    }
}

void Menubar::render_info_const(){

}

void Menubar::render_info_func(){
    ImGui::OpenPopup("Info Functions");
    ImGui::SetNextWindowSize(ImVec2(400,400));
    if (ImGui::BeginPopupModal("Info Functions", nullptr, ImGuiWindowFlags_NoResize)) {

        if (ImGui::CollapsingHeader("setUniform1i"))
            ImGui::Text("Set int uniform");
        if (ImGui::CollapsingHeader("setUniform1f"))
            ImGui::Text("Set float uniform");
        if (ImGui::CollapsingHeader("setUniform2f"))
            ImGui::Text("Set vec2 uniform");
        if (ImGui::CollapsingHeader("setUniform3f"))
            ImGui::Text("Set vec3 uniform");
        if (ImGui::CollapsingHeader("setUniform4f"))
            ImGui::Text("Set vec4 uniform");
        if (ImGui::CollapsingHeader("setCursorPos"))
            ImGui::Text("Set position of mouse cursor");
        if (ImGui::CollapsingHeader("setCursorVisibility"))
            ImGui::Text("Change visibility of mouse cursor");
        if (ImGui::CollapsingHeader("getDT"))
            ImGui::Text("Returns delta time of last frame in ms");
        if (ImGui::CollapsingHeader("getTT"))
            ImGui::Text("Returns total time since last start in ms");
        if (ImGui::CollapsingHeader("getResolutionX"))
            ImGui::Text("Returns X resolution of render texture");
        if (ImGui::CollapsingHeader("getResolutionY"))
            ImGui::Text("Returns Y resolution of render texture");
        if (ImGui::CollapsingHeader("getWindowW"))
            ImGui::Text("Returns width of window");
        if (ImGui::CollapsingHeader("getWindowH"))
            ImGui::Text("Returns height of window");

        ImGui::Separator();

        float windowWidth = ImGui::GetWindowSize().x;
        float buttonWidth = ImGui::CalcTextSize("Close").x + ImGui::GetStyle().FramePadding.x * 2;
        ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
            _info_func = false;
        }
        ImGui::EndPopup();
    }
}

void Menubar::render_delete_script() {
    ImGui::OpenPopup("Delete Script");
    ImGui::SetNextWindowSize(ImVec2(400,120));
    if (ImGui::BeginPopupModal("Delete Script", nullptr, ImGuiWindowFlags_NoResize)) {

        ImGui::TextWrapped("Are you sure you want to delete this Script? The image file in the project directory will also be deleted");

        ImGui::Separator();

        ImVec2 windowSize = ImGui::GetWindowSize();
        float buttonWidth = 175.0f;
        float buttonHeight = 30.0f;
        float spacing = 20.0f;
        float totalWidth = 2 * buttonWidth + spacing;
        float button1PosX = (windowSize.x - totalWidth) * 0.5f;
        float button2PosX = button1PosX + buttonWidth + spacing;
        ImGui::SetCursorPosY(windowSize.y - buttonHeight - ImGui::GetStyle().WindowPadding.y);

        ImGui::SetCursorPosX(button1PosX);
        if (ImGui::Button("Cancel", ImVec2(buttonWidth, buttonHeight))){
            ImGui::CloseCurrentPopup();
            _delete_script = false;
        }
        ImGui::SameLine();

        ImGui::SetCursorPosX(button2PosX);
        if (ImGui::Button("Delete", ImVec2(buttonWidth, buttonHeight))){
            Project::get()->deleteScript();
            ImGui::CloseCurrentPopup();
            _delete_script = false;
        }

        ImGui::EndPopup();
    }
}

void Menubar::render() {
    if(_delete_script)
        render_delete_script();
    if(_info_const)
        render_info_const();
    if(_info_func)
        render_info_func();

    if(ImGui::BeginMainMenuBar()){

        if(ImGui::BeginMenu("File")){

            if(ImGui::MenuItem("New", nullptr)) {
                ((View*)view)->getViewable("NewProject")->setVisible(true);
            }

            if(ImGui::MenuItem("Open", nullptr)) {
                std::string projectPath = util_open_file_manager(PROJECT_FILE_NAME);
                if(!projectPath.empty()){
                    auto* project = new Project(projectPath);
                    open_project(project);
                }
            }

            if(ImGui::BeginMenu("Resent Projects")){
                for (const auto& path : Config::get()->getRecentProjects()) {
                    if(ImGui::MenuItem(path.c_str(), nullptr)){
                        auto* project = new Project(path + "kikan.proj");
                        open_project(project);
                    }
                }
                ImGui::EndMenu();
            }

            if(ImGui::MenuItem("Close", nullptr)) {
                if(Project::get())
                    Project::get()->close();
            }
            if(ImGui::MenuItem("Exit", nullptr)) {
                exit(0);
            }

            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Edit")){
            if(ImGui::MenuItem("Viewport", nullptr)) {
                ((View*)view)->getViewable("ViewportSettings")->setVisible(true);
            }
            if(ImGui::MenuItem("Textures", nullptr)) {
                ((View*)view)->getViewable("TexturesView")->setVisible(true);
            }

            ImGui::EndMenu();
        }

        if(Project::get()){
            if(ImGui::BeginMenu("Script")){
                if(Project::get()->getScript()){
                    if(ImGui::MenuItem(Project::get()->getScript()->isActive() ? "Deactivate" : "Activate", nullptr)) {
                        Project::get()->getScript()->setActive(!Project::get()->getScript()->isActive());
                    }
                    if(ImGui::MenuItem("ReRun", nullptr)) {
                        Project::get()->getScript()->launch();
                    }
                }
                else{
                    if(ImGui::MenuItem("Add Script", nullptr)) {
                        Project::get()->createScript();
                    }
                }

                if(ImGui::BeginMenu("Help")){
                    if(ImGui::MenuItem("Constants", nullptr)) {
                        _info_const = true;
                    }
                    if(ImGui::MenuItem("Functions", nullptr)) {
                        _info_func = true;
                    }
                    ImGui::EndMenu();
                }

                if(Project::get()->getScript()){
                    if(ImGui::MenuItem("Delete Script", nullptr)) {
                        _delete_script = true;
                    }
                }
                ImGui::EndMenu();
            }



            ImGui::Separator();
            if(Project::get()->isPaused()){
                if(ImGui::MenuItem("Resume", nullptr)) {
                    Project::get()->pause(false);
                }
            }
            else{
                if(ImGui::MenuItem("Pause ", nullptr)) {
                    Project::get()->pause(true);
                }
            }
            ImGui::Separator();
        }


        ImGui::EndMainMenuBar();
    }
}
