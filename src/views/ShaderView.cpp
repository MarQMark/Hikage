#include "views/ShaderView.h"
#include "imgui/imgui.h"

#define TOP_SPACING 20.f

void ShaderView::render() {

    //ImGui::SetNextWindowPos(ImVec2(0.0f, 20.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("ShaderView", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    float view_padding = 10;
    float x_min, x_max, y_min, y_max;
    float win_x = ImGui::GetWindowPos().x;
    float win_y = ImGui::GetWindowPos().y;
    float win_w = ImGui::GetWindowWidth();
    float win_h = ImGui::GetWindowHeight() - TOP_SPACING;
    float txtAspect = 1;
    if(Project::get())
        txtAspect = (float)Project::get()->getWidth() / (float)Project::get()->getHeight();
    float winAspect = win_w / win_h;

    if(winAspect > txtAspect){
        y_min = win_y + view_padding;
        y_max = win_y + win_h - view_padding;
        x_min = win_x + (win_w - (y_max - y_min) * txtAspect) / 2.f;
        x_max = win_x + (win_w + (y_max - y_min) * txtAspect) / 2.f;
    }
    else {
        x_min = win_x + view_padding;
        x_max = win_x + win_w - view_padding;
        y_min = win_y + (win_h - (x_max - x_min) / txtAspect) / 2.f;
        y_max = win_y + (win_h + (x_max - x_min) / txtAspect) / 2.f;
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddImage(reinterpret_cast<ImTextureID>(_viewport->txt()),
                       ImVec2(x_min, y_min),
                       ImVec2(x_max, y_max),
                       ImVec2(0, 1),
                       ImVec2(1, 0));


    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::Text("FPS: %.0f", 1000 / Uniforms::get()->dt);
    ImGui::SetCursorPosX(0);
    ImGui::Text("u_delta_time: %.1f", Uniforms::get()->dt);
    ImGui::SetCursorPosX(0);
    ImGui::Text("u_time: %.0f", Uniforms::get()->tt);
    ImGui::SetCursorPosX(0);
    ImGui::Text("u_mouse: %.0f, %.0f", Uniforms::get()->mousePos.x, Uniforms::get()->mousePos.y);
    ImGui::SetCursorPosX(0);
    ImGui::Text("u_resolution: %.0f, %.0f", Uniforms::get()->dim.x, Uniforms::get()->dim.y);

    ImGui::End();
    ImGui::PopStyleVar();
}

void ShaderView::setViewport(Viewport *viewport) {
    _viewport = viewport;
}
