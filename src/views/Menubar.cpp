#include "views/Menubar.h"
#include "imgui/imgui.h"
#include "views/View.h"

void Menubar::render() {
    if(ImGui::BeginMainMenuBar()){

        if(ImGui::BeginMenu("File")){

            if(ImGui::MenuItem("New", nullptr)) {
                ((View*)view)->getViewable("NewProject")->setVisible(true);
            }

            if(ImGui::MenuItem("Open", nullptr)) {

            }

            if(ImGui::BeginMenu("Resent Projects")){

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Edit")){

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
