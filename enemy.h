#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "player.h"
#include "bullet.h"
#include "beam.h"
#include "vector2D.h"
#include "health.h"
#include "sprite.h"
#include "audio.h"
#include "graphics.h"
#include "drop.h"
#include "explosion.h"
#include "gameMode.h"

enum class enemyType {
    BULLET,
    BEAM
};

struct Player;

struct Enemy {
    enemyType type;
    Vector2D position;
    Vector2D velocity;
    Vector2D direction;
    SDL_Texture* texture;
    SDL_Rect dest;
    SDL_Rect healthBarDest;
    int damage = 2;
    int beamDamage = 1;
    int explosionDamage = 4;
    bool alive;
    float angle;

    BulletManager bullets;
    BeamManager beams;
    Health health;
    Sprite thruster;
    Audio* SFX;
    HealthBar healthBar;

    std::pair<int, int> bulletShipTypes[4] = { {0, 0}, {2, 0}, {0, 1}, {3, 1} };
    std::pair<int, int> beamShipTypes[2] = { {4, 0}, {2, 1} };
    int speed;
    bool dropped = false;
    bool exploded = false;

    float shootTimer;
    float shootCooldown;

    float hurtTimer = 0.0f;
    float hurtDuration = 0.15f;

    SDL_Rect srcRect;
    SDL_Rect bulletSrcRect;
    SDL_Rect beamSrcRect;

    Enemy (Vector2D position, SDL_Texture* texture, SDL_Rect dest, SDL_Texture* bullet, SDL_Texture* thruster, Audio* sound, enemyType eType);
    void render(SDL_Renderer* renderer, Camera &camera);
    void update(float deltaTime, Graphics& graphics, Player &player, DropManager& drops);

    void resetShootTimer();
    bool shootON();
    bool isBeamFiring() const;
};


struct EnemyManager {
    std::vector <Enemy> enemies;
    SDL_Texture* enemyTexture;
    SDL_Texture* thrusterTexture;
    SDL_Texture* bulletTexture;
    SDL_Texture* beamTexture;
    Audio* SFX;
    enemyType type;
    gameMode& mode;

    int scorePerEnemy = 10;
    int deadCount = 0;
    int getScore = 0;

    float spawnTimer = 0.0f;
    float spawnCooldown;

    int count;

    EnemyManager(gameMode& m);
    void init(Graphics& graphics, Audio& sound);

    void resetSpawnTimer();
    bool spawnON();

    Vector2D spawnEnemyOutsideCamera(Camera& camera, int margin);
    void spawn(Camera& camera);
    void update(float deltaTime, Graphics& graphics, Player &player, DropManager& drops, ExplosionManager& explosionManager);
    void render(SDL_Renderer* renderer, Camera &camera);
    void reset();
};
#endif // ENEMY_H_INCLUDED
