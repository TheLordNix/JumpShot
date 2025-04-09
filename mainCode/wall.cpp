#include "wall.hpp"
#include "entity.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

Wall::Wall(Vector2f p_pos, int p_width, int p_height, SDL_Texture* p_tex)
    : Entity(p_pos, p_tex), width(p_width), height(p_height) {
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = width;
    currentFrame.h = height;
}

int Wall::getWidth() const {
    return width;  
}

int Wall::getHeight() const {
    return height;  
}
