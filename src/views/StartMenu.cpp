#include <string>
#include "views/StartMenu.h"
#include "imgui/imgui.h"
#include "config/Config.h"
#include "util/utilOS.h"
#include "views/View.h"


void StartMenu::open_project(Project* project){
    if(project->load() == 1){
        Project::open(project);
    }
    else{
        printf("[ERROR] opening Project\n");
        delete project;
    }
}


void StartMenu::render() {
    ImGui::SetNextWindowPos(ImVec2(0.0f, 20.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("ShaderView", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);


    ImGuiIO& io = ImGui::GetIO();
    ImVec2 windowSize = io.DisplaySize;
    float buttonWidth = windowSize.x * .33f;
    float buttonHeight = windowSize.y * .1f;;
    float windowCenterX = windowSize.x * 0.5f;
    float windowCenterY = windowSize.y * 0.5f;
    float totalHeight = (buttonHeight * 2) + 10.0f;  // 10px spacing between buttons
    float startX = windowCenterX - (buttonWidth * 0.5f);
    float startY = windowCenterY - (totalHeight * 0.5f);
    ImGui::SetCursorScreenPos(ImVec2(startX, startY));

    if (ImGui::Button("New Project", ImVec2(buttonWidth, buttonHeight))) {
        ((View*)view)->getViewable("NewProject")->setVisible(true);
    }
    ImGui::SetCursorScreenPos(ImVec2(startX, ImGui::GetCursorPosY() + buttonHeight));
    if (ImGui::Button("Open Project", ImVec2(buttonWidth, buttonHeight))){
        std::string projectPath = util_open_file_manager(PROJECT_FILE_NAME);
        if(!projectPath.empty()){
            auto* project = new Project(projectPath);
            open_project(project);
        }
    }


    ImGui::End();
    ImGui::PopStyleVar();
}
