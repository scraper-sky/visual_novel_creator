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
    int scene_current { 0 }, background_current { 0 }, sprite_current { 0 };
    bool scene_panel_exit_state { false };
    void display_scene_storage(){
        int window_width { 200 }, window_height { 50 };
        if(scene_panel_exit_state){
            ImGui::SetNextWindowSize(ImVec2(window_width, window_height), ImGuiCond_FirstUseEver);
            ImGui::Begin("Scene Storage", &scene_panel_exit_state); //here, &show functions as a pointer to a static bool declared earlier 
            ImGui::Columns(3, "Uploaded files are shown here");
            ImGui::Text("Backgrounds");
            if(scene_setup_storage::background_list.size() != 0){ 
                //my setup for a draggable list of background ids; run through list to obtain selectable ones from uploaded list
                if(ImGui::BeginCombo("##bg_combo", std::filesystem::path(scene_setup_storage::background_list[background_current].file_name).filename().string().c_str())){
                    for(int i = 0; i < scene_setup_storage::background_list.size(); i++){
                        ImGui::PushID(i);
                        bool is_selected = (background_current == i); //check if current index is the one selected by the user
                        if(ImGui::Selectable(std::filesystem::path(scene_setup_storage::background_list[i].file_name).filename().string().c_str(), is_selected)){
                            background_current = i;
                        }
                        //set every background in the list as being draggable
                        if(ImGui::BeginDragDropSource()){
                            ImGui::SetDragDropPayload("BACKGROUND_ID", &i, sizeof(int));
                            ImGui::Text("Dragging: %s", std::filesystem::path(scene_setup_storage::background_list[i].file_name).filename().string().c_str());
                            ImGui::EndDragDropSource();
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
                        if(ImGui::BeginDragDropSource()){
                            ImGui::SetDragDropPayload("SPRITE_ID", &i, sizeof(int));
                            ImGui::Text("Dragging: %s", std::filesystem::path(scene_setup_storage::sprite_list[i].file_name).filename().string().c_str());
                            ImGui::EndDragDropSource();
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndCombo();
                }
            }
            ImGui::NextColumn();
            ImGui::Text("Scenes");
            if(scene_setup_storage::scene_list.size() != 0){
                std::string scene_name { "Scene " + std::to_string(scene_setup_storage::scene_list[scene_current].scene_id) };
                if(ImGui::BeginCombo("##scene_combo", scene_name.c_str())){
                    for(int i = 0; i < scene_setup_storage::scene_list.size(); i++){
                        ImGui::PushID(i);
                        bool is_selected = (scene_current == i);
                        std::string current_scene_name { "Scene " + std::to_string(scene_setup_storage::scene_list[i].scene_id) };
                        if(ImGui::Selectable(current_scene_name.c_str(), is_selected)){
                            scene_current = i;
                        }
                        if(ImGui::BeginDragDropSource()){
                            ImGui::SetDragDropPayload("SCENE_ID", &i, sizeof(int));
                            ImGui::Text("Dragging: %s", current_scene_name.c_str());
                            ImGui::EndDragDropSource();
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndCombo();
                }
            }
            ImGui::NextColumn();
            ImGui::End();
        }
    }
}

