cmake_minimum_required(VERSION 3.10)
project(visual_novel_creator)
#this is the text file that the compiler reads to compile the imgui files
set(CMAKE_CXX_STANDARD 17)

find_package(raylib REQUIRED)

set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_demo.cpp
)

add_executable(vn_creator
    main.cpp
    assets/sprites/user_sprites.cpp
    assets/backgrounds/user_background.cpp
    assets/scenes/scenes.cpp
    raylib-imgui/rlImGui.cpp
    screens/editor_screen.cpp
    screens/ui/buttons.cpp
    screens/ui/scene_panel.cpp
    tinyfiledialogs/tinyfiledialogs.c
    ${IMGUI_SOURCES}
)

target_include_directories(vn_creator PRIVATE imgui raylib-imgui tinyfiledialogs scenes)
target_link_libraries(vn_creator raylib)
