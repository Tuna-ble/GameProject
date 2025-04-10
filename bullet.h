#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED
#include "def.h"
#include<bits/stdc++.h>

struct Player;
struct Camera;

struct Bullet{
    float x, y;
    float vx, vy;
    int width, height;
    bool active;
    float angle;

    SDL_Texture* texture;  // shared texture (sprite sheet)
    SDL_Rect srcRect;      // phần hình trong sprite sheet

    Bullet(float x, float y, float vx, float vy, SDL_Texture* tex, SDL_Rect src, float angle);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, Camera& camera);
};

struct BulletManager{
    std::vector<Bullet> bullets;
    SDL_Texture* bulletTexture;
    const float bulletSpeed = 200.0f;

    void init(SDL_Texture* texture);

    void shoot(float x, float y, float dirX, float dirY, float speed, const SDL_Rect& srcRect, float angle);

    void update(float dt);

    void render(SDL_Renderer* renderer, Camera& camera);
};

#endif // BULLET_H_INCLUDED
