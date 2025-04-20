#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "background.h"
#include "vector2D.h"

enum class bulletFrom {
    PLAYER,
    ENEMY
};

struct Player;
struct Camera;

struct Bullet{
    Vector2D position;
    Vector2D velocity;
    int width, height;
    bool active;
    float angle;
    int damage;
    bulletFrom shooter;

    SDL_Texture* texture;
    SDL_Rect srcRect;

    float activeTimer = 0.0f;
    float activeDuration = 5.0f;

    Bullet(Vector2D position, Vector2D velocity, int damage, SDL_Texture* tex, SDL_Rect src, float angle, bulletFrom shooter);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, Camera& camera);
};

struct BulletManager{
    std::vector<Bullet> bullets;
    SDL_Texture* bulletTexture;
    int bulletSpeed = 200;

    void init(SDL_Texture* texture);

    void shoot(Vector2D position, Vector2D direction, int damage, float speed, const SDL_Rect& srcRect, float angle, bulletFrom shooter);

    void update(float dt);

    void render(SDL_Renderer* renderer, Camera& camera);

    Vector2D getBulletSpawnPosition(Vector2D& position) const;
};

//struct Beam {
//    Vector2D position;
//    int width, height;
//    bool active;
//    float angle;
//    int damage;
//    bulletFrom shooter;
//
//    SDL_Texture* texture;
//    SDL_Rect srcRect;
//
//    float activeTimer = 0.0f;
//    float activeDuration = 5.0f;
//
//    Bullet(Vector2D position, Vector2D velocity, int damage, SDL_Texture* tex, SDL_Rect src, float angle, bulletFrom shooter);
//    void update(float deltaTime);
//    void render(SDL_Renderer* renderer, Camera& camera);
//};

#endif // BULLET_H_INCLUDED
