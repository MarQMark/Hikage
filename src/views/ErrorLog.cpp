#include "views/ErrorLog.h"
#include "imgui/imgui.h"

void ErrorLog::render() {

    ImGui::SetNextWindowPos(ImVec2(0.0f, 20.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("ErrorLog", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

    ImGui::Text("%s", _log);

    ImGui::End();
    ImGui::PopStyleVar();
}

void ErrorLog::setLog(char *log) {
    _log = log;
}
