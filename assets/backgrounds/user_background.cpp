#include <iostream>
#include <raylib.h>
#include <filesystem>
#include "imgui.h"
#include "tinyfiledialogs.h"
#include "../scenes/scenes.h"

extern int background_counter;

namespace fs = std::filesystem;

namespace scene_setup_storage{
    extern std::vector<scene_setup::Background> background_list;
}

namespace user_background_setup{
    //implementation is logically identical to user sprites set up
    //set up button wired as upload_backgrounds

    bool upload_backgrounds_exit_state { false };

    void upload_backgrounds_button(){
        int button_window_x_pos { 300 }, button_window_y_pos { 120 }, button_window_width { 350 }, button_window_height { 25 };
        ImGui::SetNextWindowPos(ImVec2(button_window_x_pos, button_window_y_pos), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(button_window_width, button_window_height * 2), ImGuiCond_FirstUseEver);
        if(upload_backgrounds_exit_state){
            ImGui::Begin("Upload Background", &upload_backgrounds_exit_state);

            if(ImGui::Button("Upload Background")){
                //we set allowable file types and get the user's environment path/downloads
                char const * filter_patterns[2] = {"*.jpg", "*.png"};
                const char* retrieved_user_path = getenv("HOME");
                std::string appended_path { "/Downloads" }; 
                std::string retrieved_user_download_path { (std::string)retrieved_user_path + appended_path };
            
                //same as before
                //c_str() used to cast retrieved user download past from std::string to char const *
                //this function returns path to user-selected file; next we use filesystem library to implement the actual file storage
                char const * uploaded_background { tinyfd_openFileDialog("", retrieved_user_download_path.c_str(), 2, filter_patterns, "image files", 0) };
                if(uploaded_background != NULL){

                    //copy uploaded destination into a file path that we set here using the filesystem path
                    fs::path source { uploaded_background }; 
                    fs::path destination_dir("local_assets/local_backgrounds");
                    fs::path destination { destination_dir / source.filename() };

                    try{ //like before, try allows program to continue running even if an error occurs with file uploading
                        bool copied_file_to_destination = fs::copy_file(source, destination);

                        //increment background counter and add the new background into the background_list
                        if(copied_file_to_destination){
                            background_counter += 1;
                            scene_setup::Vector2 default_background_position { 0 , 0 }; 
                            Texture2D background_image = LoadTexture(destination.string().c_str());
                            float default_scale { 1.0 };
                            scene_setup::Background new_background { background_counter, (std::string) uploaded_background, default_background_position, background_image, default_scale};
                            scene_setup_storage::background_list.push_back(new_background); //add new background to global background list
                        }
                        else{
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