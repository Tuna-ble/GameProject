#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include <SDL.h>
#include <SDL_image.h>
#include "def.h"
#include "graphics.h"

#include "bullet.h"
#include "vector2D.h"
#include "health.h"
#include "sprite.h"

struct Camera;

struct Player{
    BulletManager bullets;
    Health health;
    Sprite thruster;
    Vector2D position = {startX, startY};
    Vector2D velocity;
    SDL_Texture* playerTexture;

    float healTimer = 0.0f;
    float healCooldown = 1.0f;

    float hurtTimer = 0.0f;
    float hurtDuration = 0.15f;

    float angle;
    int speed = 200;
    int damage = 2;
    bool gameRunning = true;
    const SDL_Rect srcRect = { (ID % 2) * 48, (ID / 2) * 48, 48, 48 };
    const SDL_Rect bulletSrcRect = { (ID % 3) * 500, (ID / 2) * 500, 500, 500 };

    void init(SDL_Texture* bulletTexture, SDL_Texture* thrusterTexture);
    void render(SDL_Renderer* renderer, SDL_Texture* texture, Camera &camera, int ID);
    void renderCursor();
    void handleInput(SDL_Event& event, Camera &camera);
    void update(float deltaTime, Camera &camera);
    void reset();
};

#endif // PLAYER_H_INCLUDED
