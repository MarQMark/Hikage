#include "views/ShaderView.h"
#include "imgui/imgui.h"

void ShaderView::render() {

    ImGui::SetNextWindowPos(ImVec2(0.0f, 20.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

    ImGui::End();
    ImGui::PopStyleVar();
}
