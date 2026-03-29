#include <iostream>
#include <algorithm>
#include <raylib.h>
#include <filesystem>
#include "nlohmann/json.hpp"
#include "imgui.h"
#include "rlImGui.h"
#include "../../assets/scenes/scenes.h"

extern int scene_counter; //used to link scene_counter to global scene_counter declared in scenes.cpp

using json = nlohmann::json;

//forward declaration of storage with our exact scene setup
namespace scene_setup_storage{
    extern std::vector<scene_setup::Scene> scene_list;
    extern std::vector<scene_setup::Background> background_list;
    extern std::vector<scene_setup::Sprite> sprite_list;
}

//helper base64_encode function used later to encode visual novel properly in html format
std::string base64_encode(const unsigned char* data, int len){
    const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    for(int i = 0; i < len; i += 3){
        int b = (data[i] & 0xFC) >> 2;
        result += chars[b];
        b = (data[i] & 0x03) << 4;

        if(i + 1 < len){
            b |= (data[i+1] & 0xF0) >> 4;
            result += chars[b];
            b = (data[i+1] & 0x0F) << 2;
        }
        else{
            result += chars[b];
            result += "==";
            break;
        }

        if(i + 2 < len){
            b |= (data[i+2] & 0xC0) >> 6;
            result += chars[b];
            result += chars[data[i+2] & 0x3F];
        }
        else{
            result += chars[b];
            result += "=";
            break;
        }
    }
    return result;
}

namespace create_visual_novel{

    bool create_visual_novel_exit_state { false };
    int create_button_width { 150 }, create_button_height { 50 };
    std::vector<int> visual_novel_scenes;

    void display_visual_novel_creator(){
        if(create_visual_novel_exit_state){

            ImGui::Begin("Create Visual Novel", &create_visual_novel_exit_state);
            ImGui::Columns(2, "##create_visual_novel_columns");

            ImGui::Button("Drop Scenes Here", ImVec2(create_button_width, create_button_height));
            if(ImGui::BeginDragDropTarget()){
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_ID")){
                    int scene_dropped_id { *(const int*)payload->Data };
                    visual_novel_scenes.push_back(scene_dropped_id);
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::NextColumn();

            for(int i = 0; i < visual_novel_scenes.size(); i++){
                ImGui::PushID(i);
                if(ImGui::ArrowButton("up", ImGuiDir_Up) && i > 0){
                    std::swap(visual_novel_scenes[i], visual_novel_scenes[i-1]);
                }
                ImGui::SameLine();
                if(ImGui::ArrowButton("down", ImGuiDir_Down) && i < visual_novel_scenes.size() - 1){
                    std::swap(visual_novel_scenes[i], visual_novel_scenes[i+1]);
                }
                ImGui::SameLine();
                std::string scene_label {"Scene " + std::to_string(scene_setup_storage::scene_list[visual_novel_scenes[i]].scene_id)};
                ImGui::Selectable(scene_label.c_str(), false);
                ImGui::PopID();
            }

            if(ImGui::Button("Export Visual Novel", ImVec2(create_button_width, create_button_height))){
                
                //create json version of visual novel
                json visual_novel_json = json::array();

                for(int i = 0; i < visual_novel_scenes.size(); i++){

                    scene_setup::Scene current_scene_access { scene_setup_storage::scene_list[visual_novel_scenes[i]] };

                    json current_scene;
                    current_scene["Scene ID"] = current_scene_access.scene_id;
                    current_scene["Scene Sprites"] = current_scene_access.scene_sprites[0].id; //ignore sprite list for now; implement back later
                    current_scene["Scene Background ID"] = current_scene_access.scene_background_id;
                    current_scene["Scene Dialogue"] = current_scene_access.text;
                    
                    json current_scene_scale;
                    current_scene_scale["Background Scale"] = current_scene_access.background_scale;
                    current_scene_scale["Sprite Scale"] = current_scene_access.sprite_scale;
                    current_scene["Scaling"] = current_scene_scale;

                    json current_scene_sprite_position;
                    current_scene_sprite_position["x"] = current_scene_access.sprite_x;
                    current_scene_sprite_position["y"] = current_scene_access.sprite_y;
                    current_scene["Sprite Position"] = current_scene_sprite_position;

                    visual_novel_json.push_back(current_scene);

                }
                std::string visual_novel_content = visual_novel_json.dump(4);
                SaveFileText("visual_novel.json", (char*)visual_novel_content.c_str());
                
                //now create the html version of the visual novel
                std::string html = "<!DOCTYPE html><html><head><style>";
                html += "body{margin:0;background:#000;display:flex;justify-content:center;align-items:center;height:100vh;}";
                html += ".scene{display:none;flex-direction:column;cursor:pointer;}";
                html += ".scene.active{display:flex;}";
                html += ".dialogue{width:100%;background:rgba(0,80,180,0.85);color:white;padding:20px;box-sizing:border-box;font-size:18px;}";
                html += ".sprite{position:absolute;}";
                html += "</style></head><body>";
                
                //for each individual scene from the visual novel, we retrieve the path and convert to relevant html component
                for(int i = 0; i < visual_novel_scenes.size(); i++){                                                                                                                                                              
                    scene_setup::Scene s { scene_setup_storage::scene_list[visual_novel_scenes[i]]};                                                                                                                              
                                                                                                                                                                                                                    
                    std::string bg_path = scene_setup_storage::background_list[s.scene_background_id].file_name;                                                                                                                  
                    int bg_size = 0;                                                                                                                                                                                              
                    unsigned char* bg_data = LoadFileData(bg_path.c_str(), &bg_size);                                                                                                                                             
                    std::string bg_mime = std::filesystem::path(bg_path).extension().string() == ".png" ? "image/png": "image/jpeg";                                                                                              
                    std::string bg_encoded = base64_encode(bg_data, bg_size);                                                                                                                                                     
                    int bg_w = static_cast<int>(scene_setup_storage::background_list[s.scene_background_id].background_image.width * s.background_scale);                                                                         
                    int bg_h = static_cast<int>(scene_setup_storage::background_list[s.scene_background_id].background_image.height * s.background_scale);                                                                        
                    UnloadFileData(bg_data);
                                                                                                                                                                                                                    
                    std::string sprite_path = s.scene_sprites[0].file_name;
                    int sprite_size = 0;                                                                                                                                                                                          
                    unsigned char* sprite_data = LoadFileData(sprite_path.c_str(), &sprite_size);                                                                                                                                 
                    std::string sprite_mime = std::filesystem::path(sprite_path).extension().string() == ".png" ? "image/png" : "image/jpeg";
                    std::string sprite_encoded = base64_encode(sprite_data, sprite_size);                                                                                                                                         
                    int sprite_w = static_cast<int>(s.scene_sprites[0].sprite_image.width * s.sprite_scale);
                    int sprite_h = static_cast<int>(s.scene_sprites[0].sprite_image.height * s.sprite_scale);                                                                                                                     
                    UnloadFileData(sprite_data);
                                                                                                                                                                                                                    
                    html += "<div class='scene" + std::string(i == 0 ? " active" : "") + "' style='width:" + std::to_string(bg_w) + "px;'>";                                                                      
                    html += "<div style='position:relative;width:" + std::to_string(bg_w) + "px;height:" + std::to_string(bg_h) + "px;'>";                                                                      
                    html += "<img src='data:" + bg_mime + ";base64," + bg_encoded + "' style='width:" + std::to_string(bg_w) + "px;height:" + std::to_string(bg_h) + "px;display:block;'/>";                 
                    html += "<img class='sprite' src='data:" + sprite_mime + ";base64," + sprite_encoded + "' style='position:absolute;width:" + std::to_string(sprite_w) + "px;height:" + std::to_string(sprite_h) + "px;left:" + std::to_string(s.sprite_x) + "px;top:" + std::to_string(s.sprite_y) + "px;'/>";
                    html += "</div>";                                                                                    
                    html += "<div class='dialogue'>" + s.text + "</div>";
                    html += "</div>";                                                                                                                                                                                             
                }
                
                html += "<script>";
                html += "const scenes = document.querySelectorAll('.scene');";
                html += "let current = 0;";
                html += "document.body.addEventListener('click', () => {";
                html += "scenes[current].classList.remove('active');";
                html += "current = (current + 1) % scenes.length;";
                html += "scenes[current].classList.add('active');";
                html += "});";
                html += "</script></body></html>";

                SaveFileText("visual_novel.html", (char*)html.c_str());
            }
            ImGui::Columns(1);
            ImGui::End();
        }
    }
}