#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "def.h"
#include "graphics.h"
#include "background.h"
#include "bullet.h"

struct Camera;

struct Player{
    BulletManager bullets;
    float x = 100, y = 100;
    float vx, vy;
    int speed = 200;
    bool gameRunning = true;
    const SDL_Rect srcRect = { (ID % 2) * 48, (ID / 2) * 48, 48, 48 };
    const SDL_Rect bulletSrcRect = { (ID % 3) * 500, (ID / 2) * 500, 500, 500 };

    void init(SDL_Texture* bulletTexture);
    void render(SDL_Renderer* renderer, SDL_Texture* texture, Camera &camera, int ID);
    void renderCursor();
    void handleInput(SDL_Texture* texture, Camera &camera);
    void update(float deltaTime);
};

#endif // PLAYER_H_INCLUDED
