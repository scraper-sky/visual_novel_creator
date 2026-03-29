#include <iostream>
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

//forward declaration of scene panel setup to obtain the necesary draggable sprites and backgrounds
namespace scene_panel_setup{
    extern int scene_current;
    extern int background_current;
    extern int sprite_current;
    extern bool scene_panel_exit_state;
    void display_scene_storage();
}

namespace create_scene_panel{

    bool create_scene_panel_exit_state { false };
    int background_dropped_id { -1 }, sprite_dropped_id { -1 };
    int create_button_width { 300 }, create_button_height { 120 };
    int window_width { 900 }, window_height { 600 };
    int dialogue_box_height { 100 };
    int sprite_x { 0 }, sprite_y { 0 };
    float background_image_scale { 1.0 }, sprite_image_scale { 1.0 };
    static char dialogue_text[1024] { "" };

    void display_create_scene_panel(){
        if(create_scene_panel_exit_state){
            
            ImGui::SetNextWindowSize(ImVec2(window_width, window_height), ImGuiCond_FirstUseEver);
            ImGui::Begin("Create Scene", &create_scene_panel_exit_state);
            ImGui::Columns(2, "##create_scene_columns");

            ImGui::Button("Drop Background Here", ImVec2(create_button_width, create_button_height));
            if(ImGui::BeginDragDropTarget()){
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BACKGROUND_ID")){
                    background_dropped_id = *(const int*)payload->Data;
                }
                ImGui::EndDragDropTarget();
            }


            ImGui::Button("Drop Sprite Here", ImVec2(create_button_width, create_button_height));
            if(ImGui::BeginDragDropTarget()){
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SPRITE_ID")){
                    sprite_dropped_id = *(const int *)payload->Data;
                }
                ImGui::EndDragDropTarget();
            }

            if(ImGui::Button("Reset Background", ImVec2(create_button_width, create_button_height))){
                background_dropped_id = -1;
                sprite_dropped_id = -1;
            }

            if(ImGui::Button("Export Scene", ImVec2(create_button_width, create_button_height))){
                if(background_dropped_id != -1 && sprite_dropped_id != -1){
                    scene_counter += 1;
                    std::vector<scene_setup::Sprite> scene_sprites;
                    scene_sprites.push_back(scene_setup_storage::sprite_list[sprite_dropped_id]);
                    std::string scene_text { dialogue_text };
                    scene_setup::Scene new_scene { scene_counter, scene_sprites, background_dropped_id, scene_text, background_image_scale, sprite_image_scale};
                    scene_setup_storage::scene_list.push_back(new_scene);
                    std::cout << "Added new scene to scene list";
                }
                else{
                    std::cout << "Scene failed to export";
                }
            }

            ImGui::NextColumn();

            ImGui::InputFloat("Background Scale", &background_image_scale, 0.1f, 1.0f, "%.2f");
            ImGui::InputFloat("Sprite Scale", &sprite_image_scale, 0.1f, 1.0f, "%.2f"); 

            ImGui::InputInt("Sprite X", &sprite_x); 
            ImGui::InputInt("Sprite Y", &sprite_y);
            ImGui::InputInt("Dialogue Box Height", &dialogue_box_height);


            if(background_dropped_id != -1 && sprite_dropped_id != -1){

                Texture2D background_image = scene_setup_storage::background_list[background_dropped_id].background_image;
                float scaled_background_height = static_cast<float>(background_image.height) * background_image_scale;
                float scaled_background_width = static_cast<float>(background_image.width) * background_image_scale;
                ImVec2 background_position = ImGui::GetCursorScreenPos();  
                rlImGuiImageSize(&background_image, static_cast<int>(scaled_background_width), static_cast<int>(scaled_background_height));

                Texture2D sprite_image = scene_setup_storage::sprite_list[sprite_dropped_id].sprite_image;
                float scaled_sprite_height = static_cast<float>(sprite_image.height) * sprite_image_scale;
                float scaled_sprite_width = static_cast<float>(sprite_image.width) * sprite_image_scale;
                ImGui::SetCursorScreenPos(ImVec2(background_position.x + sprite_x, background_position.y + sprite_y)); 
                rlImGuiImageSize(&sprite_image, static_cast<int>(scaled_sprite_width), static_cast<int>(scaled_sprite_height));

                ImGui::SetCursorScreenPos(ImVec2(background_position.x, background_position.y + scaled_background_height));
                ImGui::InputTextMultiline("##dialogue", dialogue_text, sizeof(dialogue_text), ImVec2(scaled_background_width, dialogue_box_height));

            }

            ImGui::Columns(1);
            ImGui::End();
        }
    }

}