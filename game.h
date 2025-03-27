#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <SDL.h>
#include <SDL_image.h>
#include "def.h"
#include "graphics.h"
#include "player.h"

struct Game{
    Graphics graphics;
    Player player;
    SDL_Texture* background;
    SDL_Texture* spaceShip;
    SDL_Texture* cursor;

    void init();
    void run();
    void render();
    void update();
    void quit();
};

#endif // GAME_H_INCLUDED
