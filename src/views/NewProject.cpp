#include <cstdio>

#ifdef __linux
    #include <dirent.h>
#endif

#include <memory>
#include <thread>
#include "views/NewProject.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "config/Config.h"
#include "views/ViewUtil.h"
#include "config/Project.h"

NewProject::NewProject() {
    _visible = false;

    _name = "NewProject";
    _path = Config::get()->projectPath();

}

void NewProject::render() {
    ImGui::OpenPopup("New Project");
    ImGui::SetNextWindowSize(ImVec2(400,270));
    if (ImGui::BeginPopupModal("New Project", nullptr, ImGuiWindowFlags_NoResize))
    {
        float windowWidth = ImGui::GetWindowSize().x;
        int errors = 0;

        centeredText("Create a new Project");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        ImGui::Text("Name");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if(ImGui::InputText("##Name", &_name)){
            _valid_name = validate_name();
        }
        if(!_valid_name && _valid_path){
            errors++;
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
            centeredText("Project already exists");
            ImGui::PopStyleColor();
        }
        else{
            ImGui::Dummy(ImVec2(0.0f, ImGui::CalcTextSize("Project already exists").y));
        }


        ImGui::Text("Location");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if(ImGui::InputText("##Location", &_path)){
            _valid_path = validate_path();
            _valid_name = validate_name();
        }
        if(!_valid_path){
            errors++;
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
            centeredText("Invalid Path");
            ImGui::PopStyleColor();
            ImGui::Dummy(ImVec2(0.0f, 20.0f - ImGui::CalcTextSize("Invalid Path").y));
        }
        else{
            ImGui::Dummy(ImVec2(0.0f, 24.0f));
        }


        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        ImGui::Separator();

        ImVec2 windowSize = ImGui::GetWindowSize();
        float buttonWidth = 175.0f;
        float buttonHeight = 30.0f;
        float spacing = 20.0f;
        float totalWidth = 2 * buttonWidth + spacing;
        float button1PosX = (windowWidth - totalWidth) * 0.5f;
        float button2PosX = button1PosX + buttonWidth + spacing;
        ImGui::SetCursorPosY(windowSize.y - buttonHeight - ImGui::GetStyle().WindowPadding.y);

        ImGui::SetCursorPosX(button1PosX);
        if (ImGui::Button("Close", ImVec2(buttonWidth, buttonHeight))){
            ImGui::CloseCurrentPopup();
            setVisible(false);
        }
        ImGui::SameLine();

        if(errors != 0)
            ImGui::BeginDisabled();
        ImGui::SetCursorPosX(button2PosX);
        if (ImGui::Button("Create", ImVec2(buttonWidth, buttonHeight))){
            create_project();
            ImGui::CloseCurrentPopup();
            setVisible(false);
        }
        if(errors != 0)
            ImGui::EndDisabled();

        ImGui::EndPopup();
    }
}

void NewProject::create_project() {
    auto* project = new Project(_name, _path);
    project->create();
    Project::open(project);
}

void NewProject::clean() {
    _name = "NewProject";
    _path = Config::get()->projectPath();
}

void NewProject::setVisible(bool visible) {
    Viewable::setVisible(visible);

    if(visible){
        _valid_path = validate_path();
        _valid_name = validate_name();
        if(_valid_path){
            for(int i = 1; i < 100 && !_valid_name; i++){
                _name = "NewProject" + std::to_string(i);
                _valid_name = validate_name();
            }
        }
    }
    else{
        clean();
    }
}

bool NewProject::validate_name() {
    if(!_valid_path)
        return false;

#ifdef __linux
    DIR *dir;
    struct dirent *entry;
    dir = opendir(_path.c_str());
    while ((entry = readdir(dir)) != nullptr) {
        if (_name == entry->d_name)
            return false;
    }
#endif

    return true;
}

bool NewProject::validate_path() {
    DIR *dir = opendir(_path.c_str());
    if (dir == nullptr){
        printf("Error could not open %s: %s\n", _path.c_str(), strerror(errno));
        return false;
    }

    return true;
}

