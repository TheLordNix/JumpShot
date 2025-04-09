#pragma once
#include <SDL2/SDL.h>
#include "entity.hpp"
#include "math.hpp"
#include "wall.hpp"

class Spikeball : public Entity {
    public:
        // Constructor
        Spikeball(Vector2f position, float velocity, int width, int height, SDL_Texture* texture);
    
        // Methods
        
        int getWidth() const;
        int getHeight() const;
        float getVel() const;
        virtual void falling(const Wall& wall); // Optional: Make this virtual if subclasses override it
        virtual void reset(); // Optional: Make this virtual if subclasses override it
    
    private:
        float vel;       // Current velocity
        int width;
        int height;
        Uint32 delayEndTime; // Time when the delay ends
    };
