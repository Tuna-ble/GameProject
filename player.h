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
#include "audio.h"
#include "gameUI.h"

struct Camera;

struct Player{
    BulletManager bullets;
    Health health;
    Sprite thruster;
    Vector2D position = {startX, startY};
    Vector2D velocity;
    SDL_Texture* playerTexture;
    Audio SFX;
    HealthBar healthBar;

    float healTimer = 0.0f;
    float healCooldown = 1.0f;

    float hurtTimer = 0.0f;
    float hurtDuration = 0.15f;

    float angle;
    int speed = 200;
    int damage = 2;
    bool gameRunning = true;
    const SDL_Rect srcRect = { (ID % 4) * 128, (ID / 2) * 128, 128, 128 };
    const SDL_Rect bulletSrcRect = { (ID % 3) * 500, (ID / 2) * 500, 500, 500 };
    SDL_Rect healthBarRect = { 20, 20, 200, 30 };

    void init(SDL_Texture* bulletTexture, SDL_Texture* thrusterTexture, Audio& SFX);
    void render(SDL_Renderer* renderer, SDL_Texture* texture, Camera &camera, int ID);
    void renderCursor();
    void handleInput(SDL_Event& event, Camera &camera);
    void update(float deltaTime, Camera &camera);
    void reset();
};

#endif // PLAYER_H_INCLUDED
