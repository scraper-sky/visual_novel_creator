#include <iostream>
#include <algorithm>
#include <raylib.h>
#include "imgui.h"
#include "rlImGui.h"
#include "../../assets/scenes/scenes.h"

extern int scene_counter; //used to link scene_counter to global scene_counter declared in scenes.cpp

//forward declaration of storage with our exact scene setup
namespace scene_setup_storage{
    extern std::vector<scene_setup::Scene> scene_list;
    extern std::vector<scene_setup::Background> background_list;
    extern std::vector<scene_setup::Sprite> sprite_list;
}

namespace create_visual_novel{

    bool create_visual_novel_exit_state { false };
    int create_button_width { 100 }, create_button_height { 40 };
    std::vector<int> visual_novel_scenes;

    void display_visual_novel_creator(){
        if(create_visual_novel_exit_state){

            ImGui::Begin("Create Visual Novel", &create_visual_novel_exit_state);
            ImGui::Columns(2, "##create_visual_novel_columns");

            ImGui::Button("Drop Scenes Here", ImVec2(create_button_width, create_button_height));
            if(ImGui::BeginDragDropTarget()){
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_ID")){
                    int scene_dropped_id { *(const int*)payload->Data };
                    visual_novel_scenes.push_back(scene_dropped_id);
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::NextColumn();

            for(int i = 0; i < visual_novel_scenes.size(); i++){
                ImGui::PushID(i);
                if(ImGui::ArrowButton("up", ImGuiDir_Up) && i > 0){
                    std::swap(visual_novel_scenes[i], visual_novel_scenes[i-1]);
                }
                ImGui::SameLine();
                if(ImGui::ArrowButton("down", ImGuiDir_Down) && i < visual_novel_scenes.size() - 1){
                    std::swap(visual_novel_scenes[i], visual_novel_scenes[i+1]);
                }
                ImGui::SameLine();

                std::string scene_label {"Scene " + std::to_string(scene_setup_storage::scene_list[visual_novel_scenes[i]].scene_id)};
                ImGui::Selectable(scene_label.c_str(), false);

                ImGui::PopID();
            }

            ImGui::Columns(1);
            ImGui::End();
        }
    }
}