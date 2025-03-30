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
                    if(ImGui::MenuItem("Delete Script", nullptr)) {
                        _delete_script = true;
                    }
                }
                else{
                    if(ImGui::MenuItem("Add Script", nullptr)) {
                        Project::get()->createScript();
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
