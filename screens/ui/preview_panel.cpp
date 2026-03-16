#include <iostream>
#include <raylib.h>
#include "imgui.h"
#include "rlImGui.h"
#include "../../assets/scenes/scenes.h"

extern int scene_counter; //used to link scene_counter to global scene_counter declafed in scenes.cpp

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
    float background_image_scale { 1.0 }, sprite_image_scale { 1.0 };
    int background_image_width { 300 }, background_image_height { 300 };
    int sprite_image_width { 50 }, sprite_image_height { 50 };

    void display_create_scene_panel(){
        if(create_scene_panel_exit_state){
            ImGui::Begin("Create Scene", &create_scene_panel_exit_state);

            ImGui::Button("Drop Background Here", ImVec2(create_button_width, create_button_height));
            if(ImGui::BeginDragDropTarget()){
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BACKGROUND_ID")){
                    background_dropped_id = *(const int*)payload->Data;
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::InputFloat("Background Scale", &background_image_scale, 0.1f, 1.0f, "%.2f");

            ImGui::Button("Drop Sprite Here", ImVec2(create_button_width, create_button_height));
            if(ImGui::BeginDragDropTarget()){
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SPRITE_ID")){
                    sprite_dropped_id = *(const int *)payload->Data;
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::InputFloat("Sprite Scale", &sprite_image_scale, 0.1f, 1.0f, "%.2f"); 

            if(background_dropped_id != -1 && sprite_dropped_id != -1){

                Texture2D background_image = scene_setup_storage::background_list[background_dropped_id].background_image;
                float scaled_background_height = static_cast<float>(background_image_height) * background_image_scale;
                float scaled_background_width = static_cast<float>(background_image_width) * background_image_scale;
                rlImGuiImageSize(&background_image, static_cast<int>(scaled_background_height), static_cast<int>(scaled_background_width));

                Texture2D sprite_image = scene_setup_storage::sprite_list[sprite_dropped_id].sprite_image;
                float scaled_sprite_height = static_cast<float>(sprite_image_height) * sprite_image_scale;
                float scaled_sprite_width = static_cast<float>(sprite_image_width) * sprite_image_scale;
                rlImGuiImageSize(&sprite_image, static_cast<int>(scaled_sprite_height), static_cast<int>(scaled_sprite_width));

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
                    std::string scene_text { "" };
                    scene_setup::Scene new_scene { scene_counter, scene_sprites, background_dropped_id, scene_text, background_image_scale, sprite_image_scale};
                    scene_setup_storage::scene_list.push_back(new_scene);
                    std::cout << "Added new scene to scene list!";
                }
                else{
                    std::cout << "Scene failed to export!";
                }
            }

            ImGui::End();
        }
    }

}