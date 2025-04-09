#include "entity.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

const float GRAVITY = 0.0000018f;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;

Entity::Entity(Vector2f p_pos, SDL_Texture* p_tex)
:pos(p_pos), vel(0, 0), acc(0, 0), tex(p_tex)// initialize all these values
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 32;
    currentFrame.h = 32;
}

SDL_Texture* Entity::getTex()
{
    return tex;
}

void Entity::setPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

void Entity::setAngle(float p_angle)
{
	angle = p_angle;
}

SDL_Rect Entity::getCurrentFrame() const
{
    return currentFrame;
}

/*
float Entity::getVelocityY()
{
	return vel.y;
}
*/

int Entity::getWidth() const
{
	return currentFrame.w;
}

int Entity::getHeight() const
{
	return currentFrame.h;
}
