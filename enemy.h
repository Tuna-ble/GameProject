#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "player.h"
#include "bullet.h"

struct Player;

struct Enemy {
    float x, y;
    float vx, vy;
    SDL_Texture* texture;
    SDL_Rect dest;
    const SDL_Rect bulletSrcRect = { (ID % 3) * 500, (ID / 2) * 500, 500, 500 };

    BulletManager bullets;

    const SDL_Rect srcRect = { (1 % 2) * 48, (1 / 2) * 48, 48, 48 };

    Enemy (float x, float y, SDL_Texture* texture, SDL_Rect dest, SDL_Texture* bullet);
    void render(SDL_Renderer* renderer, SDL_Texture* texture, Camera &camera);
    void update(float deltaTime, Player &player);
};


struct EnemyManager {
    std::vector <Enemy> enemies;
    SDL_Texture* enemyTexture;

    float shootTimer = 0.0f;
    float shootCooldown = 1.0f;

    float spawnTimer = 0.0f;
    float spawnCooldown = 5.0f;

    void init(SDL_Texture* texture);

    void resetShootTimer();
    bool shootON();
    void resetSpawnTimer();
    bool spawnON();

    void spawn(float x, float y, SDL_Texture* texture, SDL_Texture* bullet, Player &player);
    void update(float deltaTime, Player &player);
    void render(SDL_Renderer* renderer, Camera &camera);
};
#endif // ENEMY_H_INCLUDED
