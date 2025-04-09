#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "math.hpp"
#include "entity.hpp"
#include "wall.hpp"
#include "obstacle.hpp"
#include "spikeBall.hpp"
#include "powerUP.hpp"

class Obstacle;
using namespace std;

class Gun : public Entity {
public:
    Gun(Vector2f p_pos, SDL_Texture* p_tex);

    void update();
    void jump();
    void rotateLeft();
    void rotateRight();
    bool checkCollision(const Wall& wall);
    void handleCollision(const Wall& wall);
    bool checkCollision(const Obstacle& obstacle);
    void handleCollision(const Obstacle& obstacle);
    bool checkCollision(const Spikeball& spikeball);
    void handleCollision(const Spikeball& spikeball);
    bool checkCollision(const PowerUP& powerUP);
    void handleCollision(const PowerUP& powerUP);
    void setVelocity(Vector2f newVelocity);
    void setAcceleration(Vector2f newAcceleration);
    Vector2f getVelocity() const;

    const char* getScore();
    const char* getHighscore();
    int getScoreInt();
    int isDead();
    void reset(Vector2f newPosition);

private:
    int score = 0;
    int highscore = 0;
    int timer = 0;
    int dead = 0;

    Vector2f velocity; 
};