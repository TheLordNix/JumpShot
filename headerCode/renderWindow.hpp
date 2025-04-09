#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "entity.hpp"

class RenderWindow
{
public:
    RenderWindow(const char* p_title, int p_w, int p_h);
    SDL_Texture* loadTexture(const char* p_filePath);
    void render(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);
    void render(float p_x, float p_y, SDL_Texture* p_tex);
    void renderCenter(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);
    
    int getRefreshRate();
    
    void cleanUp();
    void clear();
    void render(Entity& p_entity);
    void display();

    SDL_Renderer* getRenderer() const;
private:
    SDL_Window* win;
    SDL_Renderer* rend;
};
