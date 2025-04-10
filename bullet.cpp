#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "player.h"
#include "bullet.h"

Bullet::Bullet(float x, float y, float vx, float vy,
    SDL_Texture* texture, SDL_Rect src, float angle)
    : x(x), y(y), vx(vx), vy(vy),
      width(BULLET_SIZE), height(BULLET_SIZE),
      active(true), texture(texture), srcRect(src), angle(angle) {}

void Bullet::update(float deltaTime) {
    x += vx * deltaTime;
    y += vy * deltaTime;

    // Xóa nếu ra khỏi màn hình hoặc tùy game logic
    if (x < -SCREEN_WIDTH || x > 5000 || y < -SCREEN_HEIGHT || y > 5000) {
        active = false;
    }
}

void Bullet::render(SDL_Renderer* renderer, Camera& camera) {
    if (!active) return;
    SDL_Rect dst = {
        (int)(x - camera.x),
        (int)(y - camera.y),
        width, height
    };
    SDL_RenderCopyEx(renderer, this->texture, &this->srcRect, &dst, angle, NULL, SDL_FLIP_NONE);
}

void BulletManager::init(SDL_Texture* texture) {
    bulletTexture = texture;
    }

void BulletManager::shoot(float x, float y, float dirX, float dirY, float speed, const SDL_Rect& srcRect, float angle) {
        bullets.emplace_back(x, y, dirX * speed, dirY * speed, bulletTexture, srcRect, angle);
    }

void BulletManager::update(float dt) {
        for (auto& b : bullets) b.update(dt);
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                     [](const Bullet& b) { return !b.active; }), bullets.end());
    }

void BulletManager::render(SDL_Renderer* renderer, Camera& camera) {
    for (auto& b : bullets) b.render(renderer, camera);
}
