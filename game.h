#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "def.h"
#include "graphics.h"
#include "player.h"
#include "background.h"
#include "enemy.h"
#include "collision.h"
#include "sprite.h"
#include "gameUI.h"

struct Game{
    Graphics graphics;
    Player player;
    Camera camera;
    TiledRenderer tile;
    EnemyManager enemies;
    Collision collision;
    MainMenu menu;

    TTF_Font* font;
    SDL_Texture* text;
    SDL_Texture* background;
    SDL_Texture* spaceShip;
    SDL_Texture* cursor;
    SDL_Texture* bullet;
    SDL_Texture* enemy;
    SDL_Texture* sprite;

    bool startGame;
    bool quitGame;

    void init();
    void run();
    void render();
    void update(float deltaTime);
    void quit();
};

#endif // GAME_H_INCLUDED
