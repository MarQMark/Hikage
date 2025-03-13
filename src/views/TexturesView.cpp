#include "views/TexturesView.h"

#include <config/Project.h>
#include <imgui/imgui.h>
#include <util/utilOS.h>

#include "imgui/imgui_stdlib.h"
#include <views/ViewUtil.h>

TexturesView::TexturesView() {
    _visible = false;
}

void TexturesView::render() {
    if(_render_new_txt) {
        render_new_txt();
        return;
    }
    if(_render_del_txt){
        render_del_txt();
        return;
    }

    ImGui::OpenPopup("Textures Settings");
    ImGui::SetNextWindowSize(ImVec2(400,600));
    if (ImGui::BeginPopupModal("Textures Settings", nullptr, ImGuiWindowFlags_NoResize)) {
        centeredText("Textures");
        ImVec2 windowSize = ImGui::GetWindowSize();

        if(Project::get()) {

            ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.05f);
            ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.95f, 470), ImGuiChildFlags_Border);
            for(size_t i = 0; i < Project::get()->getTextures().size(); i++) {
                auto* txt = Project::get()->getTextures()[i];
                ImGui::PushID(i);
                ImGui::Text("%s", txt->getName().c_str());

                ImGui::BeginGroup();
                ImGui::Image(reinterpret_cast<uint32_t *>(txt->getID()),
                    ImVec2(64, 64),
                    ImVec2(0.0f, 0.0f),
                    ImVec2(1.0f, 1.0f),
                    ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                    ImGui::GetStyleColorVec4(ImGuiCol_Border));
                if (ImGui::BeginItemTooltip())
                {
                    ImGui::Text("Width:  %d", txt->getWidth());
                    ImGui::Text("Height: %d", txt->getHeight());
                    ImGui::EndTooltip();
                }

                ImGui::EndGroup();
                ImGui::SameLine();

                ImGui::BeginGroup();
                ImGui::Text("Sampler2D Name:");
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::InputText("##sampler", txt->getRSampler());
                if(ImGui::Button("Delete", ImVec2(ImGui::GetContentRegionAvail().x, 25))) {
                    _render_del_txt = true;
                    _del_txt_idx = (int)i;
                }
                ImGui::EndGroup();
                ImGui::Separator();
                ImGui::PopID();
            }
            ImGui::EndChild();


            ImGui::SetCursorPosX((windowSize.x - 250) / 2);
            if (ImGui::Button("New Texture", ImVec2(250, 30))){
                _render_new_txt = true;
            }
        }
        else {
            centeredText("No Project open");
        }

        ImGui::SetCursorPosY(windowSize.y - 30 - ImGui::GetStyle().WindowPadding.y);
        ImGui::SetCursorPosX((windowSize.x - 300) / 2);
        if (ImGui::Button("Close", ImVec2(300, 30))){
            Project::get()->save();
            ImGui::CloseCurrentPopup();
            setVisible(false);
        }
        ImGui::SameLine();

        ImGui::EndPopup();
    }
}

void TexturesView::render_new_txt() {
    ImGui::OpenPopup("New Texture");
    ImGui::SetNextWindowSize(ImVec2(400,200));
    if (ImGui::BeginPopupModal("New Texture", nullptr, ImGuiWindowFlags_NoResize)) {

        int errors = 0;
        centeredText("Path to Image:");
        ImGui::Dummy(ImVec2(0, 20));

        ImGui::SetNextItemWidth(355);
        ImGui::InputText("##Path", &_new_txt_path);
        ImGui::SameLine();
        if(ImGui::Button("O", ImVec2(20, 20))) {
            _new_txt_path = util_open_file_manager("*.png");
            printf("%s\n", _new_txt_path.c_str());
        }
        ImGui::Dummy(ImVec2(0, 20));

        if(_new_txt_path.empty() || !std::filesystem::exists(_new_txt_path)){
            errors++;
            centeredText("Invalid Path");
        }
        else if(std::filesystem::is_directory(_new_txt_path)) {
            errors++;
            centeredText("Not a File");
        }
        else {
            size_t pos = _new_txt_path.rfind('/');

            if (pos != std::string::npos) {
                std::string name = _new_txt_path.substr(pos + 1, _new_txt_path.length() - 1);
                for(auto* txt : Project::get()->getTextures()) {
                    if(txt->getName() == name) {
                        errors++;
                        centeredText("Texture with same Name already exists");
                        break;
                    }
                }
            }
            else {
                errors++;
                centeredText("Unknown Error");
            }
        }

        if(errors == 0) {
            ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeight()));
        }
        ImGui::Dummy(ImVec2(0, 10));
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
        if (ImGui::Button("Close", ImVec2(buttonWidth, buttonHeight))){
            ImGui::CloseCurrentPopup();
            _new_txt_path = "";
            _render_new_txt = false;
        }
        ImGui::SameLine();

        if(errors != 0)
            ImGui::BeginDisabled();
        ImGui::SetCursorPosX(button2PosX);
        if (ImGui::Button("Create", ImVec2(buttonWidth, buttonHeight))){
            auto* txt = new Texture(_new_txt_path);
            if(txt->create(_new_txt_path) != -1)
                Project::get()->addTexture(txt, _new_txt_path);
            else
                delete txt;

            Project::get()->save();
            ImGui::CloseCurrentPopup();
            _new_txt_path = "";
            _render_new_txt = false;
        }
        if(errors != 0)
            ImGui::EndDisabled();

        ImGui::EndPopup();
    }
}

void TexturesView::render_del_txt() {
    ImGui::OpenPopup("Delete Texture");
    ImGui::SetNextWindowSize(ImVec2(400,120));
    if (ImGui::BeginPopupModal("Delete Texture", nullptr, ImGuiWindowFlags_NoResize)) {

        ImGui::TextWrapped("Are you sure you want to delete this texture? The image file in the project directory will also be deleted");

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
            _render_del_txt = false;
        }
        ImGui::SameLine();

        ImGui::SetCursorPosX(button2PosX);
        if (ImGui::Button("Delete", ImVec2(buttonWidth, buttonHeight))){
            Project::get()->delTexture(_del_txt_idx);
            Project::get()->save();
            ImGui::CloseCurrentPopup();
            _del_txt_idx = -1;
            _render_del_txt = false;
        };

        ImGui::EndPopup();
    }
}

