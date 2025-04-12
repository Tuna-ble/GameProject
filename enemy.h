#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "player.h"
#include "bullet.h"
#include "vector2D.h"

struct Player;

struct Enemy {
    Vector2D position;
    Vector2D velocity;
    SDL_Texture* texture;
    SDL_Rect dest;
    bool alive;

    BulletManager bullets;

    float shootTimer = 0.0f;
    float shootCooldown = 1.0f;

    SDL_Rect srcRect;
    SDL_Rect bulletSrcRect;

    Enemy (Vector2D position, SDL_Texture* texture, SDL_Rect dest, SDL_Texture* bullet);
    void render(SDL_Renderer* renderer, SDL_Texture* texture, Camera &camera);
    void update(float deltaTime, Player &player);

    void resetShootTimer();
    bool shootON();
};


struct EnemyManager {
    std::vector <Enemy> enemies;
    SDL_Texture* enemyTexture;

    float spawnTimer = 0.0f;
    float spawnCooldown = 5.0f;

    void init(SDL_Texture* texture);

    void resetSpawnTimer();
    bool spawnON();

    void spawn(Vector2D position, SDL_Texture* texture, SDL_Texture* bullet, Player &player);
    void update(float deltaTime, Player &player);
    void render(SDL_Renderer* renderer, Camera &camera);
};
#endif // ENEMY_H_INCLUDED
