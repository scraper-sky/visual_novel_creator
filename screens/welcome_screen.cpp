#include <iostream>
#include <raylib.h>
#include "rlImGui.h"

int window_width { 1000 };
int window_height { 800 };
bool is_in_welcome_screen { false };
bool should_switch_to_editor { false };
bool should_switch_to_welcome_screen { false };
Color chosen_color { LIGHTGRAY };

//declaring namespaces as forward declarations to avoid the circular dependency problem I encountered before
namespace button_setup {
    void starting_window_buttons();
}

namespace start_window_setup{

    void initialize_window(){
        InitWindow(window_width, window_height, "Visual Novel Creator");
        is_in_welcome_screen = true ;
    }

    void show_user_cursor_settings(){
        ShowCursor();
    }

    Color toggle_color(Color input_color){
        return ColorToInt(input_color) == ColorToInt(LIGHTGRAY) ? DARKGRAY : LIGHTGRAY;
    }

    void window_screen_interface(){
        Color title_color = { 255, 100, 100, 255 };
        const int title_size { 60 }, title_x_coord { 200 }, title_y_coord { 100 };
        BeginDrawing();
        ClearBackground(chosen_color);
            rlImGuiBegin();
            show_user_cursor_settings();
            DrawText("Welcome to VN Creator", title_x_coord, title_y_coord, title_size, title_color);
            button_setup::starting_window_buttons();
            rlImGuiEnd();
        EndDrawing();
    }

    void game_loop(){
        while (!WindowShouldClose() && !should_switch_to_editor){
            window_screen_interface();
        }
    }
    
    void close_window(){
        CloseWindow();
        is_in_welcome_screen = false;
    }
}