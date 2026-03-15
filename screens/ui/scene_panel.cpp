#include <iostream>
#include <raylib.h>
#include <filesystem>
#include "imgui.h"
#include "rlImGui.h"
#include "../../assets/scenes/scenes.h"

//forward declaration of storage with our exact scene setup
namespace scene_setup_storage{
    extern std::vector<scene_setup::Scene> scene_list;
    extern std::vector<scene_setup::Background> background_list;
    extern std::vector<scene_setup::Sprite> sprite_list;
}

namespace scene_panel_setup{
    static int scene_current { 0 };
    static int background_current { 0 };
    static int sprite_current { 0 };
    void display_scene_storage(){
        int window_width { 200 }, window_height { 50 };
        ImGui::SetNextWindowSize(ImVec2(window_width, window_height), ImGuiCond_FirstUseEver);
        ImGui::Begin("Scene Storage");
        ImGui::Columns(3, "Uploaded files are shown here");
        ImGui::Text("Backgrounds");
        if(scene_setup_storage::background_list.size() != 0){ 
            //my setup for a draggable list of background ids; run through list to obtain selectable ones from uploaded list
            if(ImGui::BeginCombo("##bg_combo", std::filesystem::path(scene_setup_storage::background_list[background_current].file_name).filename().string().c_str())){
                for(int i = 0; i < scene_setup_storage::background_list.size(); i++){
                    ImGui::PushID(i);
                    bool is_selected = (background_current == i);
                    if(ImGui::Selectable(std::filesystem::path(scene_setup_storage::background_list[i].file_name).filename().string().c_str(), is_selected)){
                        background_current = i;
                    }
                    ImGui::PopID();
                }
                ImGui::EndCombo();
            }
        }
        ImGui::NextColumn();
        ImGui::Text("Sprites");
        if(scene_setup_storage::sprite_list.size() != 0){
            //same structure as background list storage; set-up for sprite list
            if(ImGui::BeginCombo("##sprite_combo", std::filesystem::path(scene_setup_storage::sprite_list[sprite_current].file_name).filename().string().c_str())){
                for(int i = 0; i < scene_setup_storage::sprite_list.size(); i++){
                    ImGui::PushID(i);
                    bool is_selected = (sprite_current == i);
                    if(ImGui::Selectable(std::filesystem::path(scene_setup_storage::sprite_list[i].file_name).filename().string().c_str(), is_selected)){
                        sprite_current = i;
                    }
                    ImGui::PopID();
                }
                ImGui::EndCombo();
            }
        }
        ImGui::NextColumn();
        ImGui::Text("Scenes");
        ImGui::NextColumn();
        ImGui::End();
    }
}

