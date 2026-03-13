#include <iostream>
#include <vector>
#include "../scenes/scenes.h"

int scene_counter { 0 }, sprite_counter { 0 }, background_counter { 0 };

namespace scene_setup_storage{
    std::vector<scene_setup::Scene> scene_list;
    std::vector<scene_setup::Background> background_list;
    std::vector<scene_setup::Sprite> sprite_list;
}