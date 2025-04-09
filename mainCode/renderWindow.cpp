#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "renderWindow.hpp"
#include "entity.hpp"

using namespace std;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 480;

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
    :win(NULL), rend(NULL)
{
    win = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED, p_w,
                        p_h, SDL_WINDOW_SHOWN);
    
    const int SCREEN_WIDTH = p_w;
    const int SCREEN_HEIGHT = p_h;

    if (win == NULL)
    {
        cout << "Could not create window: " << SDL_GetError() << endl;
    }

    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    if (rend == NULL)
    {
        cout << "Could not create renderer: " << SDL_GetError() << endl;
    }
}

SDL_Texture* RenderWindow::loadTexture(const char *p_filePath)
{
    SDL_Texture* tex = IMG_LoadTexture(rend, p_filePath);
    if (tex == NULL)
    {
        cout << "Failed to load texture. Error: " << SDL_GetError() << endl;
    }
    return tex;
}

int RenderWindow::getRefreshRate()
{
    int displayIndex = SDL_GetWindowDisplayIndex(win);

    SDL_DisplayMode mode;
    if (SDL_GetDisplayMode(displayIndex, 0, &mode) != 0)
    {
        cout << "Failed to get display mode: " << SDL_GetError() << endl;
        return 60; // Default refresh rate
    }

    return mode.refresh_rate;
}

void RenderWindow::cleanUp()
{
    if (rend) SDL_DestroyRenderer(rend);
    if (win) SDL_DestroyWindow(win);
}

void RenderWindow::clear()
{
    SDL_RenderClear(rend);
}

void RenderWindow::render(Entity& p_entity)
{
    SDL_Rect src = p_entity.getCurrentFrame();
    SDL_Rect dst = {
        static_cast<int>(p_entity.getPos().x * 4), // Scale position
        static_cast<int>(p_entity.getPos().y * 4), // Scale position
        src.w * 4, // Scale width
        src.h * 4  // Scale height
    };

    SDL_Point center = { dst.w / 2, dst.h / 2 };
    SDL_RenderCopyEx(rend, p_entity.getTex(), &src, &dst, p_entity.getAngle(), &center, SDL_FLIP_NONE);
}

void RenderWindow::display()
{
    SDL_RenderPresent(rend);
}

void RenderWindow::render(float p_x, float p_y, SDL_Texture* p_tex)
{
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h); 

    SDL_Rect dst = { static_cast<int>(p_x), static_cast<int>(p_y), src.w, src.h };

    SDL_RenderCopy(rend, p_tex, &src, &dst);
}

void RenderWindow::render(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
{
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, textColor);
    if (surfaceMessage == NULL)
    {
        cout << "Failed to render text surface: " << TTF_GetError() << endl;
        return;
    }

    SDL_Texture* message = SDL_CreateTextureFromSurface(rend, surfaceMessage);
    if (message == NULL)
    {
        cout << "Failed to create text texture: " << SDL_GetError() << endl;
        SDL_FreeSurface(surfaceMessage);
        return;
    }

    SDL_Rect src = { 0, 0, surfaceMessage->w, surfaceMessage->h };
    SDL_Rect dst = { static_cast<int>(p_x), static_cast<int>(p_y), src.w, src.h };

    SDL_RenderCopy(rend, message, &src, &dst);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
}

void RenderWindow::renderCenter(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
{
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, textColor);
    if (surfaceMessage == nullptr) {
        std::cout << "Failed to render text surface: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* message = SDL_CreateTextureFromSurface(rend, surfaceMessage);
    if (message == nullptr) {
        std::cout << "Failed to create text texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surfaceMessage);
        return;
    }

    SDL_Rect src = { 0, 0, surfaceMessage->w, surfaceMessage->h };

    SDL_Rect dst;
    dst.x = (800 / 2) - (src.w / 2) + static_cast<int>(p_x); 
    dst.y = (720 / 2) - (src.h / 2) + static_cast<int>(p_y);  
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopy(rend, message, &src, &dst);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
}

SDL_Renderer* RenderWindow::getRenderer() const {
    return rend;
}