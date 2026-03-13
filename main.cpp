#include <iostream>
#include <raylib.h>
#include "screens/welcome_screen.cpp"
#include "rlImGui.h"

extern int window_width;
extern int window_height;
extern bool is_in_welcome_screen;
extern bool should_switch_to_editor;
extern bool should_switch_to_welcome_screen;

//forward declaration of editor window setup to call main_loop opening the editor window in main()
namespace start_window_setup{
    void initialize_window();
    void game_loop();
}

namespace editor_window_setup{
    void main_loop();
}

void start_editor(){ //editor loop that initializes the welcome window
    std::cout << "Visual Novel Creator Starting...\n"; 
    start_window_setup::initialize_window();
    rlImGuiSetup(true); //start rlImGui
    start_window_setup::game_loop();
    rlImGuiShutdown();
    start_window_setup::close_window();
}

int main(){

    start_editor();

    if(should_switch_to_editor){ //button to access editor pressed -> activates boolean here
        editor_window_setup::main_loop();
    }

    if(should_switch_to_welcome_screen){ //button to return to main menu pressed -> activates boolean to switch back
        is_in_welcome_screen = true;
        should_switch_to_editor = false;
        start_editor();
    }

    return 0;
}