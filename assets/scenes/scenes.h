#ifndef SCENES_H
#define SCENES_H

#include <raylib.h>

extern int scene_counter, sprite_counter, background_counter;

namespace scene_setup{

    struct Vector2{
        int x;
        int y;
    };

    struct Sprite{
        int id;
        std::string file_name;
        Vector2 position;
        Texture2D sprite_image;
        float scale;
    };

    struct Background{
        int id;
        std::string file_name;
        Vector2 position;
        Texture2D background_image;
        float scale;
    };

    struct Scene {
        int scene_id;
        std::vector<scene_setup::Sprite> scene_sprites;
        int scene_background_id;
        std::string text;
        float background_scale;
        float sprite_scale;
    };
    
}

#endif