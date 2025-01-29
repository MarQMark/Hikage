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

void Menubar::render() {
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

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
