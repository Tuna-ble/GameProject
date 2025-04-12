#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "def.h"
#include "graphics.h"
#include "background.h"
#include "bullet.h"
#include "vector2D.h"

#undef position

struct Camera;

struct Player{
    BulletManager bullets;
    Vector2D position = {100, 100};
    Vector2D velocity;
    SDL_Texture* playerTexture;

    float angle;
    int speed = 200;
    bool gameRunning = true;
    const SDL_Rect srcRect = { (ID % 2) * 48, (ID / 2) * 48, 48, 48 };
    const SDL_Rect bulletSrcRect = { (ID % 3) * 500, (ID / 2) * 500, 500, 500 };

    void init(SDL_Texture* bulletTexture);
    void render(SDL_Renderer* renderer, SDL_Texture* texture, Camera &camera, int ID);
    void renderCursor();
    void handleInput(SDL_Texture* texture, Camera &camera);
    void update(float deltaTime, Camera &camera);
};

#endif // PLAYER_H_INCLUDED
