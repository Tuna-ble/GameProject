#ifndef GAMEUI_H_INCLUDED
#define GAMEUI_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include "graphics.h"

struct MainMenu {
    SDL_Texture* backgroundTexture;
    SDL_Texture* titleText;
    SDL_Texture* playText;
    SDL_Texture* settingsText;
    SDL_Texture* quitText;

    TTF_Font* font;
    SDL_Color textColor = {255, 255, 255};
    SDL_Rect titleRect, playButton, quitButton, settingsButton;
    bool playSelected = false;
    bool mouseInPlay;
    bool mouseInSettings;
    bool mouseInQuit;
    bool gameRunning;

    bool menuON = true;

    void init(Graphics& graphics);
    void handleEvent(bool& startGame, bool& quitGame);
    void render(SDL_Renderer* renderer);
    void cleanUp();
};

struct PauseMenu {

};

struct HUD {

};

#endif // GAMEUI_H_INCLUDED
