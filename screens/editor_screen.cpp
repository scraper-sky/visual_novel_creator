#include <iostream>
#include <raylib.h>
#include "imgui.h"
#include "rlImGui.h"

extern int window_width;
extern int window_height;
extern bool is_in_welcome_screen;
extern bool should_switch_to_editor;
extern bool should_switch_to_welcome_screen;
extern Color chosen_color;

//forward declaration for button setup to avoid circular dependency loop in a file using its own functions 
namespace button_setup{
    void starting_window_buttons();
    void editor_window_file_buttons();
}

//forward declaration for start window setup to reference previous function of close_window
namespace start_window_setup{
    void close_window();
}

//setup for editor window
namespace editor_window_setup{
    
    void initialize_editor_window(){
        InitWindow(window_width, window_height, "Visual Novel Editor");
    }

    void show_user_cursor_settings(){
        ShowCursor();
    }

    void window_screen_interface(){
        BeginDrawing();
        ClearBackground(chosen_color);
            rlImGuiBegin();
            show_user_cursor_settings();
            button_setup::editor_window_file_buttons();
            rlImGuiEnd();
        EndDrawing();
    }

    void game_loop(){
        while (!WindowShouldClose() && !should_switch_to_welcome_screen){
            window_screen_interface();
        }
    }

    void close_window(){
        CloseWindow();
    }

    void main_loop(){
        editor_window_setup::initialize_editor_window();
        rlImGuiSetup(true);
        editor_window_setup::game_loop();
        rlImGuiShutdown();
        editor_window_setup::close_window();
    }

}