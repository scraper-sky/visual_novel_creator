#include <iostream>
#include <raylib.h>
#include <filesystem>
#include "imgui.h"
#include "tinyfiledialogs.h"
#include "../scenes/scenes.h"

extern int sprite_counter;

namespace fs = std::filesystem;

namespace scene_setup_storage{
    extern std::vector<scene_setup::Sprite> sprite_list;
}

namespace user_sprites_setup{

    bool upload_sprites_exit_state { false };

    void upload_sprite_button(){
        // set up button wired as upload sprite

        int button_window_x_pos { 300 }, button_window_y_pos { 120 }, button_window_width { 350 }, button_window_height { 25 };

        ImGui::SetNextWindowPos(ImVec2(button_window_x_pos, button_window_y_pos), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(button_window_width, button_window_height * 2), ImGuiCond_FirstUseEver);

        if(upload_sprites_exit_state){
            ImGui::Begin("Upload Sprite", &upload_sprites_exit_state);

            if(ImGui::Button("Upload Sprite")){
                char const * filter_patterns[2] = {"*.jpg", "*.png"}; //for now, 2 file types (jpg and png) are allowed)
                const char* retrieved_user_path = getenv("HOME");
            
                std::string appended_path { "/Downloads" };
                std::string retrieved_user_download_path { (std::string)retrieved_user_path  + appended_path };

                //c_str() used to cast retrieved user download past from std::string to char const *
                //this function returns path to user-selected file; next we use filesystem library to implement the actual file storage
                char const * uploaded_sprite { tinyfd_openFileDialog("", retrieved_user_download_path.c_str(), 2, filter_patterns, "image files", 0) };
                if(uploaded_sprite != NULL){

                    //we copy uploaded path into a local destination folder using filesystem path 
                    fs::path source { uploaded_sprite };
                    fs::path destination_dir("local_assets/local_sprites");
                    fs::path destination { destination_dir / source.filename() };

                    try { //try allows program to continue running even if an error occurs with file uploading
                        bool copied_file_to_destination = fs::copy_file(source, destination);

                        //increment sprite counter and add the new sprite into the sprite_list
                        if(copied_file_to_destination){
                            sprite_counter += 1;
                            scene_setup::Vector2 default_sprite_position { 0 , 0 };
                            Texture2D sprite_image = LoadTexture(destination.string().c_str());
                            float default_scale { 1.0 };
                            scene_setup::Sprite new_sprite {sprite_counter, (std::string) uploaded_sprite, default_sprite_position, sprite_image, default_scale};
                            scene_setup_storage::sprite_list.push_back(new_sprite); //add uploaded sprite to global sprites list
                            std::cout << "New sprite added into sprite list!";
                        }
                        else {
                            std::cout << "File not handled";
                        }
                    }
                    catch(fs::filesystem_error const& ex){
                        std::cerr << "Filesystem error: " << ex.what() << std::endl;
                    }
                }
            }
            ImGui::End();
        }
    }

}