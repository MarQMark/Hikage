#include "views/ViewUtil.h"
#include "imgui/imgui.h"

void centeredText(const char* text) {
    ImVec2 textSize = ImGui::CalcTextSize(text);
    float windowWidth = ImGui::GetWindowSize().x;
    float textOffset = (windowWidth - textSize.x) * 0.5f;
    if (textOffset > 0.0f) {
        ImGui::SetCursorPosX(textOffset);
    }
    ImGui::Text("%s", text);
}