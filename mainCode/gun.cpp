#include "entity.hpp"
#include "gun.hpp"
#include "wall.hpp"
#include "obstacle.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>


const float GRAVITY = 0.000072f;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;
const float JUMP_FORCE = 3.0f;
const float BOUNCE_DAMPENING = 0.9f;
const float MAX_DOWNWARD_ACCELERATION = 0.05f;

bool isJumping;
float turnAngle;

Gun::Gun(Vector2f p_pos, SDL_Texture* p_tex)
    : Entity(p_pos, p_tex)
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 32;
    currentFrame.h = 32;
}

void Gun::jump()
{
    float radianAngle = angle * M_PI / 180.0f; // angle to radians    
    vel.y = -JUMP_FORCE* std::cos(radianAngle);
    vel.x = JUMP_FORCE * std::sin(radianAngle) / 1.2f; 
    turnAngle = 370.0f - angle;

    isJumping = true; 
}

void Gun::update()
{
    pos.y += vel.y;
    vel.y += acc.y;
    pos.x += vel.x;
    vel.x += acc.x;

    // Apply gravity
    acc.y += GRAVITY;

    // Clamp the downward acceleration
    if (acc.y > MAX_DOWNWARD_ACCELERATION) {
        acc.y = MAX_DOWNWARD_ACCELERATION;
    }

    timer++;
    score = timer / 50; 
    if (score > highscore) {
        highscore = score;
    }

    if (isJumping) {
        float rotationSpeed = 6.0f; 
        if (std::abs(turnAngle - angle) < rotationSpeed) {
            // If close to target, snap to it and stop rotating
            angle = turnAngle;
            isJumping = false; 
        } else {
            if (turnAngle > angle)
                angle += rotationSpeed;
            else
                angle -= rotationSpeed;
        }
    }

    if (angle >= 360.0f)
        angle -= 360.0f;
    else if (angle < 0.0f)
        angle += 360.0f;

    if (pos.y > 720 || pos.x < 0 || pos.x > 1280) {
        cout << "Game Over! You fell off the edge." << endl;
        dead = 1;
    }
}

void Gun::rotateLeft()
{
    angle += 8.0f;
        if (angle >= 360.0f)
            angle -= 360.0f;
}

void Gun::rotateRight()
{
    angle -= 8.0f;
        if (angle < 0.0f)
            angle += 360.0f;
}

bool Gun::checkCollision(const Wall& wall) {
    return !(pos.x + currentFrame.w < wall.getPos().x || // sees if gun pos+ its width crosses wall
             pos.x > wall.getPos().x + wall.getWidth()  // sees if gun is already in wall by seeing wall pos+ its width
            );
}

void Gun::handleCollision(const Wall& wall) {
    if (checkCollision(wall)) {
        if (pos.x + currentFrame.w > wall.getPos().x &&
            pos.x < wall.getPos().x + wall.getWidth()) {
            vel.x = -vel.x * BOUNCE_DAMPENING;
            if (pos.x < wall.getPos().x) {
                pos.x = wall.getPos().x - currentFrame.w; // Prevent overlapping
            } else {
                pos.x = wall.getPos().x + wall.getWidth();
            }
        }
    }
}

bool Gun::checkCollision(const Obstacle& obstacle) {
    return !(pos.x + currentFrame.w < obstacle.getPos().x ||  // Gun is to the left of the obstacle
             pos.x > obstacle.getPos().x + obstacle.getWidth() ||  // Gun is to the right of the obstacle
             pos.y + currentFrame.h < obstacle.getPos().y ||  // Gun is above the obstacle
             pos.y > obstacle.getPos().y + obstacle.getHeight());  // Gun is below the obstacle
}

void Gun::handleCollision(const Obstacle& obstacle) {
    if (checkCollision(obstacle)) {
        // Determine the overlap on each axis
        float overlapX = std::min(pos.x + currentFrame.w, obstacle.getPos().x + obstacle.getWidth()) -
                         std::max(pos.x, obstacle.getPos().x);
        float overlapY = std::min(pos.y + currentFrame.h, obstacle.getPos().y + obstacle.getHeight()) -
                         std::max(pos.y, obstacle.getPos().y);

        // Check which axis has the smaller overlap
        if (overlapX < overlapY) {
            // Horizontal collision (left or right side)
            if (pos.x < obstacle.getPos().x) {
                // Hit the left side of the obstacle
                pos.x = obstacle.getPos().x - currentFrame.w;  // Adjust position to prevent overlap
            } else {
                // Hit the right side of the obstacle
                pos.x = obstacle.getPos().x + obstacle.getWidth();
            }
            vel.x = -vel.x * BOUNCE_DAMPENING;  // Invert X velocity
        } else {
            // Vertical collision (top or bottom side)
            if (pos.y < obstacle.getPos().y) {
                // Hit the top of the obstacle
                pos.y = obstacle.getPos().y - currentFrame.h;  // Adjust position to prevent overlap
            } else {
                // Hit the bottom of the obstacle
                pos.y = obstacle.getPos().y + obstacle.getHeight();
            }
            vel.y = -vel.y * BOUNCE_DAMPENING;  // Invert Y velocity
        }
    }
}

bool Gun::checkCollision(const Spikeball& spikeball) {
    return !(pos.x + currentFrame.w < spikeball.getPos().x ||  // Gun is to the left of the SpikeBall
             pos.x > spikeball.getPos().x + spikeball.getWidth() ||  // Gun is to the right of the SpikeBall
             pos.y + currentFrame.h < spikeball.getPos().y ||  // Gun is above the SpikeBall
             pos.y > spikeball.getPos().y + spikeball.getHeight());  // Gun is below the SpikeBall
}

void Gun::handleCollision(const Spikeball& spikeball) {
    if (checkCollision(spikeball)) {
        std::cout << "Game Over! You hit a SpikeBall." << std::endl;
        dead = 1; // Set the player as dead
    }
}

bool Gun::checkCollision(const PowerUP& powerUP) {
    return !(pos.x + currentFrame.w < powerUP.getPos().x ||  // Gun is to the left of the PowerUP
             pos.x > powerUP.getPos().x + powerUP.getWidth() ||  // Gun is to the right of the PowerUP
             pos.y + currentFrame.h < powerUP.getPos().y ||  // Gun is above the PowerUP
             pos.y > powerUP.getPos().y + powerUP.getHeight());  // Gun is below the PowerUP
}

void Gun::handleCollision(const PowerUP& powerUP) {
    if (checkCollision(powerUP)) {
        std::cout << "PowerUP collected!" << std::endl;
        score += 50; // Example: Add 50 points to the score
        vel.x *= 1.2f; // Example: Increase horizontal velocity
        vel.y *= 1.2f; // Example: Increase vertical velocity
    }
}

void Gun::reset(Vector2f newPosition) {
    pos = Vector2f(90, 50); 
    vel = Vector2f(0, 0);       
    acc = Vector2f(0, 0);       
    dead = 0;                   
    timer = 0;                  
    score = 0;                  
    angle = 0.0f;               
    isJumping = false;          
}


const char* Gun::getScore()
{
	std::string s = std::to_string(score);
	s = "SCORE: " + s;
	return s.c_str();
}

const char* Gun::getHighscore()
{
	std::string s = std::to_string(highscore);
	s = "BEST: " + s;
	return s.c_str();
}

int Gun::getScoreInt()
{
	return score;
}

int Gun::isDead()
{
	return dead;
}

void Gun::setVelocity(Vector2f newVelocity) {
    vel = newVelocity;
}

void Gun::setAcceleration(Vector2f newAcceleration) {
    acc = newAcceleration;
}

Vector2f Gun::getVelocity() const {
    return velocity;
}