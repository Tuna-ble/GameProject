#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "def.h"
#include "graphics.h"
#include "player.h"
#include "background.h"
#include "enemy.h"
#include "collision.h"
#include "sprite.h"
#include "gameUI.h"
#include "gameState.h"
#include "audio.h"
#include "asteroid.h"

struct Game{
    Graphics graphics;
    Player player;
    Camera camera;
    TiledRenderer tile;
    EnemyManager enemies;
    AsteroidManager asteroids;
    Collision collision;
    MainMenu mainMenu;
    PauseMenu pauseMenu;
    SettingsMenu settingsMenu;
    GameOver gameOver;
    HUD hud;

    Audio musicAndSFX;

    TTF_Font* font;
    SDL_Texture* text;
    SDL_Texture* background;
    SDL_Texture* spaceShip;
    SDL_Texture* cursor;
    SDL_Texture* bullet;
    SDL_Texture* enemy;
    SDL_Texture* asteroid;
    SDL_Texture* sprite;
    SDL_Texture* healthBar;
    SDL_Texture* health;

    gameState currentState;

    bool isRunning = true;
    int score;

    Game();
    void init();
    void run();
    void restart();
    void logicRun();
    void render();
    void update(float deltaTime);
    void quit();
};

#endif // GAME_H_INCLUDED
