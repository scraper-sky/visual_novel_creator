#include <iostream>
#include <raylib.h>
#include "imgui.h"
#include "rlImGui.h"

extern bool is_in_welcome_screen;
extern bool should_switch_to_editor;
extern bool should_switch_to_welcome_screen;
extern Color chosen_color;


//forward declaration of welcome_screen and editor_screen functions
//doing so avoids the circular dependency problem I encountered with #include editor_screen.cpp
namespace start_window_setup{
    Color toggle_color(Color input_color);
    void close_window();
}

namespace editor_window_setup{
    void initialize_editor_window();
    void main_loop();
}

namespace scene_panel_setup{
    extern bool scene_panel_exit_state;
    void display_scene_storage();
}

namespace user_sprites_setup{
    extern bool upload_sprites_exit_state;
    void upload_sprite_button();
}

namespace user_background_setup{
    extern bool upload_backgrounds_exit_state;
    void upload_backgrounds_button();
}

namespace button_setup{

    void show_editor_window(){
        should_switch_to_editor = true;
    }

    void starting_window_buttons(){

        int button_window_x_pos { 300 }, button_window_y_pos { 120 }, button_window_width { 350 }, button_window_height { 25 };
        static bool show_window { true };

        if(show_window){
            ImGui::SetNextWindowPos(ImVec2(button_window_x_pos, button_window_y_pos), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(button_window_width, button_window_height * 2), ImGuiCond_FirstUseEver);
            ImGui::Begin("Editor Window");
            ImGui::Text("Begin Creating!");

            //below are the functions with the buttons that (i) show the button (ii) when the button is pressed, the function executes
            if(ImGui::Button("Editor", ImVec2(button_window_width, button_window_height))){ 
                show_window = false;
                show_editor_window();
            }
            if(ImGui::Button("Toggle Light Mode", ImVec2(button_window_width, button_window_height))){
                chosen_color = start_window_setup::toggle_color(chosen_color);
            }
            if(ImGui::Button("Exit", ImVec2(button_window_width, button_window_height))){
                show_window = false;
                start_window_setup::close_window();
            }

            ImGui::End();
        }
    }

    void editor_window_file_buttons(){

        int button_window_x_pos { 300 }, button_window_y_pos { 120 }, button_window_width { 350 }, button_window_height { 25 };

        static bool show_window { true };
        static bool show_scene_storage { false }, show_user_sprite_upload { false }, show_user_background_upload { false };

        if(show_window){

            ImGui::SetNextWindowPos(ImVec2(button_window_x_pos, button_window_y_pos), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(button_window_width, button_window_height * 2), ImGuiCond_FirstUseEver);
            ImGui::Begin("Scene Editor");
            ImGui::Text("Scene Editor Main Menu");

            //access boolean states to display new windows; we display and upload when the button is pressed
            if(show_scene_storage){
                scene_panel_setup::display_scene_storage();
            }
            if(show_user_sprite_upload){
                user_sprites_setup::upload_sprite_button();
            }
            if(show_user_background_upload){
                user_background_setup::upload_backgrounds_button();
            }

            //buttons stored here; when pressed, the Boolean's are set to true and the display/upload functions are rendered true
            if(ImGui::Button("Access Scene Storage", ImVec2(button_window_width, button_window_height))){
                show_scene_storage = true;
                scene_panel_setup::scene_panel_exit_state = true;
            }
            if(ImGui::Button("Upload Sprites", ImVec2(button_window_width, button_window_height))){
                show_user_sprite_upload = true;
                user_sprites_setup::upload_sprites_exit_state = true;
            }
            if(ImGui::Button("Upload Backgrounds", ImVec2(button_window_width, button_window_height))){
                show_user_background_upload = true;
                user_background_setup::upload_backgrounds_exit_state = true;
            }
            if(ImGui::Button("Toggle Light Mode", ImVec2(button_window_width, button_window_height))){
                chosen_color = start_window_setup::toggle_color(chosen_color);
            }
            if(ImGui::Button("Return to Main Menu", ImVec2(button_window_width, button_window_height))){
                should_switch_to_welcome_screen = true;
            }

            ImGui::End();
        }
    }

}