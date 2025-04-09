#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
using namespace std;

#include "renderWindow.hpp"
#include "entity.hpp"
#include "Utils.hpp"
#include "gun.hpp"
#include "wall.hpp"
#include "obstacle.hpp" 
#include "spikeBall.hpp"
#include "powerUP.hpp"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "Error initializing SDL: " << SDL_GetError() << endl;
        return -1;
    }

    if (TTF_Init() == -1) {
        cout << "Error initializing SDL_ttf: " << TTF_GetError() << endl;
        SDL_Quit();
        return -1;
    }

    RenderWindow win("GAME v0.1", 800, 720);

    cout << "Loading textures..." << endl;
    SDL_Texture* sticker = win.loadTexture("C:/Users/Suryanshu/projects/trials/JumpShot/wall.png");
    SDL_Texture* playerTex = win.loadTexture("C:/Users/Suryanshu/projects/trials/JumpShot/171.png");
    SDL_Texture* highscoreBox = win.loadTexture("C:/Users/Suryanshu/projects/trials/JumpShot/highscore_box.png");
    SDL_Texture* deathOverlay = win.loadTexture("C:/Users/Suryanshu/projects/trials/JumpShot/death_overlay.png");
    SDL_Texture* obstacleTex = win.loadTexture("C:/Users/Suryanshu/projects/trials/JumpShot/wall.png");
    SDL_Texture* otherTex = win.loadTexture("C:/Users/Suryanshu/projects/trials/JumpShot/17.png");

    if (!sticker || !playerTex || !highscoreBox || !deathOverlay || !obstacleTex) {
        cout << "Error loading textures. Exiting..." << endl;
        SDL_Quit();
        return -1;
    }

    cout << "Loading fonts..." << endl;
    TTF_Font* font32 = TTF_OpenFont("C:/Users/Suryanshu/projects/trials/JumpShot/cocogoose.ttf", 32);
    TTF_Font* font32_outline = TTF_OpenFont("C:/Users/Suryanshu/projects/trials/JumpShot/cocogoose.ttf", 32);
    TTF_Font* font24 = TTF_OpenFont("C:/Users/Suryanshu/projects/trials/JumpShot/cocogoose.ttf", 24);
    TTF_Font* font16 = TTF_OpenFont("C:/Users/Suryanshu/projects/trials/JumpShot/cocogoose.ttf", 16);

    if (!font32 || !font32_outline || !font24 || !font16) {
        cout << "Error loading fonts: " << TTF_GetError() << endl;
        SDL_Quit();
        return -1;
    }

    SDL_Color white = {255, 255, 255};
    SDL_Color black = {0, 0, 0};

    Gun player(Vector2f(90, 50), playerTex);

    vector<Wall> walls = {
        Wall(Vector2f(0, 0), 32, 720 / 4, sticker),          // Left wall
        Wall(Vector2f(800 / 4 - 32, 0), 32, 720 / 4, sticker) // Right wall
    };

    vector<Obstacle*> obstacles = {
        new Obstacle(Vector2f(400, 0), 1.0f, 60, 15, obstacleTex),
        new Obstacle(Vector2f(200, -100), 0.8f, 30, 70, obstacleTex),
        new Obstacle(Vector2f(600, -200), 0.4f, 50, 20, obstacleTex)
    };
    
    vector<Spikeball*> spikeballs = {
        new Spikeball(Vector2f(400, 0), 1.8f, 15, 15, otherTex)
    };
    
    vector<PowerUP*> powerUps = {
        new PowerUP(Vector2f(400, 0), 0.3f, 20, 20, otherTex)
    };

    // Reset obstacles
    for (Obstacle* obstacle : obstacles) {
        obstacle->reset();
    }

    // Reset SpikeBalls
    for (Spikeball* spikeball : spikeballs) {
        spikeball->reset();
    }

    // Reset PowerUPs
    for (PowerUP* powerUP : powerUps) {
        powerUP->reset();
    }

    bool gameRunning = true;
    bool titleScreen = true;
    bool mainMenu = true;
    SDL_Event event;

    const float timeStep = 0.01f;
    float accumulator = 0.0f;
    float currentTime = utils::hireTimeInSeconds();

    Uint32 lastJumpTime = 0;

    cout << "Starting game loop..." << endl;

    while (gameRunning) {
        int startTicks = SDL_GetTicks();

        if (titleScreen) {
            win.clear();
        
            SDL_SetRenderDrawBlendMode(win.getRenderer(), SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(win.getRenderer(), 0, 0, 0, 164);
            SDL_Rect overlay = {0, 0, 800, 720};
            SDL_RenderFillRect(win.getRenderer(), &overlay);
        
            win.renderCenter(0, -24, "Welcome to JumpShot!", font32, white);
            win.renderCenter(0, 24, "Press Space or Click to Continue", font24, white);
            win.display();
        
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    gameRunning = false;
                    break;
                }
        
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    titleScreen = false;
                    mainMenu = true;
                }
        
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                    titleScreen = false;
                    mainMenu = true;
                }
            }
        
            if (titleScreen) {
                SDL_Delay(16); 
                continue;      
            }
        }

        if (mainMenu) {
            win.clear();
            for (Wall& wall : walls) {
                win.render(wall);
            }
            win.render(player);
        
            // Render translucent overlay for the main menu
            SDL_SetRenderDrawBlendMode(win.getRenderer(), SDL_BLENDMODE_BLEND); 
            SDL_SetRenderDrawColor(win.getRenderer(), 0, 0, 0, 164); 
            SDL_Rect overlay = {0, 0, 800, 720}; // Updated for new screen width and height
            SDL_RenderFillRect(win.getRenderer(), &overlay);
        
            win.renderCenter(0, 24, "Press Space or Click to Start", font24, white);
            win.display();
        
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    gameRunning = false;
                    break;
                }
        
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    mainMenu = false; 
                }
        
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                    mainMenu = false; 
                }
            }
        
            if (mainMenu) {
                SDL_Delay(16); 
                continue; 
            }
        }

        float newTime = utils::hireTimeInSeconds();
        float frameTime = newTime - currentTime;
        currentTime = newTime;

        accumulator += frameTime;

        while (accumulator >= timeStep) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    gameRunning = false;
                }

                Uint32 currentTime = SDL_GetTicks();

                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {

                    if (currentTime - lastJumpTime >= 300) { 
                        player.jump();
                        lastJumpTime = currentTime;
                    }
                }

                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_SPACE: 
                            if (currentTime - lastJumpTime >= 300) { 
                                player.jump();
                                lastJumpTime = currentTime; 
                            }
                            break;

                        case SDLK_d:
                        case SDLK_RIGHT:
                            player.rotateRight();
                            break;
                        case SDLK_a:
                        case SDLK_LEFT:
                            player.rotateLeft();
                            break;
                        default:
                            break;
                    }
                }
            }
            // Render obstacles
        for (Obstacle* obstacle : obstacles) {
            obstacle->falling(walls[0]); // Update the obstacle's position
            win.render(*obstacle);      // Render the obstacle
        }

        // Render SpikeBalls
        for (Spikeball* spikeball : spikeballs) {
            spikeball->falling(walls[0]); // Update the SpikeBall's position
            win.render(*spikeball);       // Render the SpikeBall
        }

        // Render PowerUPs
        for (PowerUP* powerUP : powerUps) {
            powerUP->falling(walls[0]); // Update the PowerUP's position
            win.render(*powerUP);       // Render the PowerUP
        }
            accumulator -= timeStep;
        }

        // Ensure no overlay is rendered during the main gameplay loop
        if (!mainMenu && !titleScreen && player.isDead() == 0) {
            win.clear();

            for (Wall& wall : walls) {
                win.render(wall);
            }

            // Render obstacles
            for (Obstacle* obstacle : obstacles) {
                obstacle->falling(walls[0]); // Update the obstacle's position
                win.render(*obstacle);      // Render the obstacle
            }

            // Render SpikeBalls
            for (Spikeball* spikeball : spikeballs) {
                spikeball->falling(walls[0]); // Update the SpikeBall's position
                win.render(*spikeball);       // Render the SpikeBall
            }

            // Render PowerUPs
            for (PowerUP* powerUP : powerUps) {
                powerUP->falling(walls[0]); // Update the PowerUP's position
                win.render(*powerUP);       // Render the PowerUP
            }

            win.render(player);
            player.update();

            for (Wall& wall : walls) {
                player.handleCollision(wall);
            }

            // Handle collisions with obstacles
            for (Obstacle* obstacle : obstacles) {
                if (player.checkCollision(*obstacle)) {
                    player.handleCollision(*obstacle);
                }
            }

            // Handle collisions with SpikeBalls
            for (Spikeball* spikeball : spikeballs) {
                if (player.checkCollision(*spikeball)) {
                    player.handleCollision(*spikeball);
                }
            }

            // Handle collisions with PowerUPs
            for (PowerUP* powerUP : powerUps) {
                if (player.checkCollision(*powerUP)) {
                    player.handleCollision(*powerUP);
                }
            }

            win.render(22, 20, player.getScore(), font32_outline, black);
            win.render(25, 23, player.getScore(), font32, white);
            win.render(0, 65, highscoreBox);
            win.render(25, 64, player.getHighscore(), font16, white);

            win.display();
        }

        if (player.isDead() != 0) {
            SDL_SetRenderDrawBlendMode(win.getRenderer(), SDL_BLENDMODE_BLEND); 
            SDL_SetRenderDrawColor(win.getRenderer(), 0, 0, 0, 164); 
            SDL_Rect overlay = {0, 0, 800, 720}; // Updated for new screen width and height
            SDL_RenderFillRect(win.getRenderer(), &overlay);
            
            win.renderCenter(0, -24, "Try jumping more...", font24, white);
            win.renderCenter(0, 12, "Press Space/Click to Restart or M to go to Main Menu", font16, white);

            SDL_Rect mainMenuButton = {800 / 2 - 100, 720 / 2 + 100, 200, 50}; // Centered horizontally
            SDL_SetRenderDrawColor(win.getRenderer(), 255, 255, 255, 255);
            SDL_RenderFillRect(win.getRenderer(), &mainMenuButton);
            SDL_SetRenderDrawColor(win.getRenderer(), 0, 0, 0, 255);
            SDL_RenderDrawRect(win.getRenderer(), &mainMenuButton);
            win.renderCenter(0, 125, "Main Menu", font16, black);

            win.display();
        
            bool waitingForInput = true;
            while (waitingForInput) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        gameRunning = false;
                        waitingForInput = false;
                        break;
                    }
        
                    if (event.type == SDL_KEYDOWN) {
                        switch (event.key.keysym.sym) {
                            case SDLK_SPACE:
                                waitingForInput = false;
                                player.reset(Vector2f(40, 50)); // Reset the player's position
                                // Reset obstacles
                                for (Obstacle* obstacle : obstacles) {
                                    obstacle->reset();
                                }

                                // Reset SpikeBalls
                                for (Spikeball* spikeball : spikeballs) {
                                    spikeball->reset();
                                }

                                // Reset PowerUPs
                                for (PowerUP* powerUP : powerUps) {
                                    powerUP->reset();
                                }
                                accumulator = 0.0f;
                                break;
                    
                            case SDLK_m:
                                waitingForInput = false;
                                titleScreen = true;
                                player.reset(Vector2f(40, 50)); // Reset the player's position
                                // Reset obstacles
                                for (Obstacle* obstacle : obstacles) {
                                    obstacle->reset();
                                }

                                // Reset SpikeBalls
                                for (Spikeball* spikeball : spikeballs) {
                                    spikeball->reset();
                                }

                                // Reset PowerUPs
                                for (PowerUP* powerUP : powerUps) {
                                    powerUP->reset();
                                }
                                break;
                    
                            default:
                                break;
                        }
                    }
        
                    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;
                    
                        if (mouseX >= mainMenuButton.x && mouseX <= mainMenuButton.x + mainMenuButton.w &&
                            mouseY >= mainMenuButton.y && mouseY <= mainMenuButton.y + mainMenuButton.h) {
                            waitingForInput = false;
                            mainMenu = true;
                            player.reset(Vector2f(40, 50)); // Reset the player's position
                            // Reset obstacles
                            for (Obstacle* obstacle : obstacles) {
                                obstacle->reset();
                            }

                            // Reset SpikeBalls
                            for (Spikeball* spikeball : spikeballs) {
                                spikeball->reset();
                            }

                            // Reset PowerUPs
                            for (PowerUP* powerUP : powerUps) {
                                powerUP->reset();
                            }
                        } else {
                            waitingForInput = false;
                            player.reset(Vector2f(40, 50)); // Reset the player's position
                            // Reset obstacles
                            for (Obstacle* obstacle : obstacles) {
                                obstacle->reset();
                            }

                            // Reset SpikeBalls
                            for (Spikeball* spikeball : spikeballs) {
                                spikeball->reset();
                            }

                            // Reset PowerUPs
                            for (PowerUP* powerUP : powerUps) {
                                powerUP->reset();
                            }
                            accumulator = 0.0f;
                        }
                    }
                }
                SDL_Delay(16);
            }
        }

        int frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < 1000 / win.getRefreshRate()) {
            SDL_Delay(1000 / win.getRefreshRate() - frameTicks);
        }
    }

    // Delete obstacles
    for (Obstacle* obstacle : obstacles) {
        delete obstacle;
    }
    obstacles.clear();

    // Delete SpikeBalls
    for (Spikeball* spikeball : spikeballs) {
        delete spikeball;
    }
    spikeballs.clear();

    // Delete PowerUPs
    for (PowerUP* powerUP : powerUps) {
        delete powerUP;
    }
    powerUps.clear();

    SDL_DestroyTexture(sticker);
    SDL_DestroyTexture(playerTex);
    SDL_DestroyTexture(highscoreBox);
    TTF_CloseFont(font32);
    TTF_CloseFont(font32_outline);
    TTF_CloseFont(font24);
    TTF_CloseFont(font16);
    TTF_Quit();
    win.cleanUp();
    SDL_Quit();

    return 0;
}