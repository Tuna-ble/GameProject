#ifndef GAMEUI_H_INCLUDED
#define GAMEUI_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include "graphics.h"
#include "gameState.h"

struct UIButton {
    SDL_Rect rect;
    SDL_Texture* text;
    TTF_Font* font;
    bool mouseHover = false;

    void init(Graphics& graphics, const char* textname, TTF_Font* textFont, SDL_Color color, SDL_Rect _rect);
    void render(SDL_Renderer* renderer);
    void updateHover(int mouseX, int mouseY);
    bool isClicked(SDL_Event& e);
};

struct MainMenu {
    gameState& state;
    SDL_Texture* backgroundTexture;
    SDL_Texture* titleText;

    UIButton playButton;
    UIButton settingsButton;
    UIButton quitButton;

    TTF_Font* font;
    SDL_Color textColor = {255, 255, 255};
    SDL_Rect titleRect;
    bool playSelected = false;

    bool menuON = true;

    MainMenu(gameState& s);
    void init(Graphics& graphics, TTF_Font* textFont);
    void handleEvent(SDL_Event& e, int mouseX, int mouseY);
    void render(SDL_Renderer* renderer);
    void cleanUp();
    void run();
};

struct PauseMenu {
    gameState& state;
    SDL_Texture* backgroundTexture;
    SDL_Texture* pausedText;

    UIButton resumeButton;
    UIButton settingsButton;
    UIButton mainMenuButton;
    UIButton quitButton;

    TTF_Font* font;
    SDL_Color textColor = {255, 255, 255};
    SDL_Rect pausedRect;
    bool playSelected = false;

    bool menuON = true;

    PauseMenu(gameState& s);
    void init(Graphics& graphics, TTF_Font* textFont);
    void handleEvent(SDL_Event& e, int mouseX, int mouseY);
    void render(SDL_Renderer* renderer);
    void cleanUp();
};

struct HUD {

};

#endif // GAMEUI_H_INCLUDED
