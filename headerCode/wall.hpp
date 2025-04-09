#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "math.hpp"
#include "entity.hpp"

class Wall : public Entity {
public:
    Wall(Vector2f p_pos, int p_width, int p_height, SDL_Texture* p_tex);

    int getWidth() const;
    int getHeight() const;

private:
    int width;
    int height;
};