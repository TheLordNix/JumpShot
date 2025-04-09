#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "math.hpp"

using namespace std;

class Wall;

class Entity
{
    public:
        Entity(Vector2f p_pos, SDL_Texture* p_tex);
        const Vector2f& getPos() const
        {
            return pos;
        }
        float getAngle() const
        {
            return angle;
        }

        int getWidth() const;
        int getHeight() const;

        void setPos(float x, float y);
	    void setAngle(float angle);
        
        //float getVelocityY();

        SDL_Texture* getTex();
        SDL_Rect getCurrentFrame() const;
    protected:
        Vector2f pos;
        Vector2f vel;
        Vector2f acc;
	    float angle = 0;
        SDL_Rect currentFrame;
        SDL_Texture* tex;
};