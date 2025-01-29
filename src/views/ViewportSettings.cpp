#include <string>
#include <sstream>
#include "views/ViewportSettings.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "views/ViewUtil.h"

void parse_val(int* dest, std::string& src){
    if(src.empty())
        return;

    int tmp = *dest;
    try {
        tmp = std::stoi(src);
    }
    catch (const std::invalid_argument& e) {}
    catch (const std::out_of_range& e) {}

    *dest = tmp;
}

void ViewportSettings::render() {
    ImGui::OpenPopup("Viewport Settings");
    ImGui::SetNextWindowSize(ImVec2(400,215));
    if (ImGui::BeginPopupModal("Viewport Settings", nullptr, ImGuiWindowFlags_NoResize))
    {
        float windowWidth = ImGui::GetWindowSize().x;
        int errors = 0;

        std::stringstream ss;
        ss << _width;
        std::string buf0(ss.str());
        ss.str(std::string());
        ss << _height;
        std::string buf1(ss.str());

        ImGui::SetCursorPos(ImVec2(10, 40));
        centeredText("Width");
        ImGui::SetNextItemWidth(windowWidth - ImGui::GetStyle().WindowPadding.x * 2);
        ImGui::InputText("##Width", &buf0, ImGuiInputTextFlags_CharsDecimal);
        ImGui::SetCursorPos(ImVec2(10, 100));
        centeredText("Height:");
        ImGui::SetNextItemWidth(windowWidth - ImGui::GetStyle().WindowPadding.x * 2);
        ImGui::InputText("##Height", &buf1, ImGuiInputTextFlags_CharsDecimal);

        parse_val(&_width, buf0);
        parse_val(&_height, buf1);

        GLint maxTextureSize;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        if(_width <= 0 || _width > maxTextureSize){
            errors++;
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
            centeredText("Invalid Width");
            ImGui::PopStyleColor();
            ImGui::Dummy(ImVec2(0.0f, 20.0f - ImGui::CalcTextSize("Invalid Width").y));
        }
        else if(_height <= 0 || _height > maxTextureSize){
            errors++;
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
            centeredText("Invalid Height");
            ImGui::PopStyleColor();
            ImGui::Dummy(ImVec2(0.0f, 20.0f - ImGui::CalcTextSize("Invalid Height").y));
        }
        else{
            ImGui::Dummy(ImVec2(0.0f, 24.0f));
        }

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
        if (ImGui::Button("Update", ImVec2(buttonWidth, buttonHeight))){
            _viewport->update(_width, _height);
            ImGui::CloseCurrentPopup();
            setVisible(false);
        }
        if(errors != 0)
            ImGui::EndDisabled();

        ImGui::EndPopup();
    }
}

void ViewportSettings::setViewport(Viewport *viewport) {
    _viewport = viewport;
}

void ViewportSettings::setVisible(bool visible) {
    Viewable::setVisible(visible);
    if(Project::get()){
        _width = Project::get()->getWidth();
        _height = Project::get()->getHeight();
    }
    else{
        _width = 1280;
        _height = 720;
    }
}

ViewportSettings::ViewportSettings() {
    _visible = false;
}
