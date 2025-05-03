#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include <SDL.h>
#include <SDL_image.h>
#include "def.h"
#include "graphics.h"
#include "bullet.h"
#include "beam.h"
#include "vector2D.h"
#include "health.h"
#include "sprite.h"
#include "audio.h"
#include "gameUI.h"
#include "dropType.h"
#include "skill.h"

struct Camera;

struct Player {
    BulletManager bullets;
    BeamManager beams;
    Health health;
    Sprite thruster;
    Vector2D position;
    Vector2D velocity;
    SDL_Texture* playerTexture;
    Audio* SFX;
    HealthBar healthBar;
    Shield shield;

    float healTimer = 0.0f;
    float healCooldown = 1.5f;

    float hurtTimer = 0.0f;
    float hurtDuration = 0.15f;

    float beamDamageInterval = 0.15f;
    float beamDamageTimer = 0.0f;

    float beamTimer = 0.0f;
    float beamCooldown = 15.0f;

    float angle;
    int speed = BASE_SPEED;
    int damage = 2;
    bool gameRunning = true;

    int startX, startY;

    const SDL_Rect srcRect = { (playerID % 4) * 128, (playerID / 2) * 128, 128, 128 };
    const SDL_Rect bulletSrcRect = { (playerID % 3) * 500, (playerID / 2) * 500, 500, 500 };
    const SDL_Rect healthBarRect = { 20, 20, 200, 30 };
    const SDL_Rect beamRect = { 2 * 500, 1 * 500, 500, 500};

    void init(Graphics& graphics, Audio& SFX);
    void render(SDL_Renderer* renderer, Camera &camera);
    void handleInput(SDL_Event& event, Camera &camera);
    void update(float deltaTime, Camera &camera);
    void getBuff(int value, dropType type);
    void reset();
};

#endif // PLAYER_H_INCLUDED
