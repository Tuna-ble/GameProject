#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "player.h"
#include "bullet.h"
#include "vector2D.h"
#include "health.h"
#include "sprite.h"
#include "audio.h"
#include "graphics.h"
#include "drop.h"

struct Player;

struct Enemy {
    Vector2D position;
    Vector2D velocity;
    SDL_Texture* texture;
    SDL_Rect dest;
    SDL_Rect healthBarDest;
    int damage = 2;
    bool alive;

    BulletManager bullets;
    Health health;
    Sprite thruster;
    Audio* SFX;
    HealthBar healthBar;
    Sprite explosion;

    std::pair<int, int> shipTypes[4] = { {0, 0}, {2, 0}, {0, 1}, {3, 1} };
    int speed;

    float shootTimer;
    float shootCooldown;

    float hurtTimer = 0.0f;
    float hurtDuration = 0.15f;

    SDL_Rect srcRect;
    SDL_Rect bulletSrcRect;

    Enemy (Vector2D position, SDL_Texture* texture, SDL_Rect dest, SDL_Texture* bullet, SDL_Texture* thruster, Audio* sound);
    void render(SDL_Renderer* renderer, Camera &camera);
    void update(float deltaTime, Graphics& graphics, Player &player, DropManager& drops);

    void resetShootTimer();
    bool shootON();
};


struct EnemyManager {
    std::vector <Enemy> enemies;
    SDL_Texture* enemyTexture;
    SDL_Texture* thrusterTexture;
    SDL_Texture* bulletTexture;
    Audio* SFX;

    int deadCount = 0;
    int getScore = 0;

    float spawnTimer = 0.0f;
    float spawnCooldown;

    void init(Graphics& graphics, Audio& sound);

    void resetSpawnTimer();
    bool spawnON();

    Vector2D spawnEnemyOutsideCamera(Camera& camera, int margin = 200);
    void spawn(Camera& camera);
    void update(float deltaTime, Graphics& graphics, Player &player, DropManager& drops);
    void render(SDL_Renderer* renderer, Camera &camera);
    void reset();
};
#endif // ENEMY_H_INCLUDED
