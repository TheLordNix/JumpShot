#include "wall.hpp"
#include "entity.hpp"
#include "gun.hpp"
#include "obstacle.hpp"
#include "spikeBall.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

#include "spikeBall.hpp"

Spikeball::Spikeball(Vector2f p_pos, float p_vel, int p_width, int p_height, SDL_Texture* p_tex)
    : Entity(p_pos, p_tex), vel(p_vel), width(p_width), height(p_height) {
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = width;
    currentFrame.h = height;
}

int Spikeball::getWidth() const {
    return width;  
}

int Spikeball::getHeight() const {
    return height;  
}

float Spikeball::getVel() const {
    return vel;  
}

    void Spikeball::falling(const Wall& wall) {
        if (SDL_GetTicks() < delayEndTime) {
            return; // Do nothing until the delay ends
        }
    
        pos.y += vel;
        if (pos.y > 760 / 4) {
            pos.y = -200;
            pos.x = 40 + ((rand() % (750 - 100 - 32 * 4)) / 4);
            if (pos.x > (700 - width * 4) / 4) {
                pos.x = (700 - width * 4 - 40) / 4;
            }
        }
    }

void Spikeball::reset() {
    pos.y = -200; // Reset the y-coordinate
    delayEndTime = SDL_GetTicks() + (rand() % 20000 + 10000); // Set a random delay between 1 and 40 seconds
}