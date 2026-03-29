#include <iostream>
#include <algorithm>
#include <raylib.h>
#include "nlohmann/json.hpp"
#include "imgui.h"
#include "rlImGui.h"
#include "../../assets/scenes/scenes.h"

extern int scene_counter; //used to link scene_counter to global scene_counter declared in scenes.cpp

using json = nlohmann::json;

//forward declaration of storage with our exact scene setup
namespace scene_setup_storage{
    extern std::vector<scene_setup::Scene> scene_list;
    extern std::vector<scene_setup::Background> background_list;
    extern std::vector<scene_setup::Sprite> sprite_list;
}

namespace create_visual_novel{

    bool create_visual_novel_exit_state { false };
    int create_button_width { 150 }, create_button_height { 50 };
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

            if(ImGui::Button("Export Visual Novel", ImVec2(create_button_width, create_button_height))){

                json visual_novel_json = json::array();

                for(int i = 0; i < visual_novel_scenes.size(); i++){

                    scene_setup::Scene current_scene_access { scene_setup_storage::scene_list[visual_novel_scenes[i]] };

                    json current_scene;
                    current_scene["Scene ID"] = current_scene_access.scene_id;
                    current_scene["Scene Sprites"] = current_scene_access.scene_sprites[0].id; //ignore sprite list for now; implement back later
                    current_scene["Scene Background ID"] = current_scene_access.scene_background_id;
                    current_scene["Scene Dialogue"] = current_scene_access.text;
                    
                    json current_scene_scale;
                    current_scene_scale["Background Scale"] = current_scene_access.background_scale;
                    current_scene_scale["Sprite Scale"] = current_scene_access.sprite_scale;
                    current_scene["Scaling"] = current_scene_scale;

                    json current_scene_sprite_position;
                    current_scene_sprite_position["x"] = current_scene_access.sprite_x;
                    current_scene_sprite_position["y"] = current_scene_access.sprite_y;
                    current_scene["Sprite Position"] = current_scene_sprite_position;

                    visual_novel_json.push_back(current_scene);

                }
                std::string visual_novel_content = visual_novel_json.dump(4);
                SaveFileText("visual_novel.json", (char*)visual_novel_content.c_str());
            }

            ImGui::Columns(1);
            ImGui::End();
        }
    }
}