#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"

#define speed 150

Enemy::Enemy (float x, float y, SDL_Texture* texture, SDL_Rect dest, SDL_Texture* bullet)
    : x(x), y(y), texture(texture), dest(dest) {
        bullets.init(bullet);
    }

void Enemy::render(SDL_Renderer* renderer, SDL_Texture* texture, Camera &camera) {
    float angle = atan2(vy, vx) * 180 / M_PI + 90;

    SDL_Rect drawRect = {
        static_cast<int>(x - camera.x),
        static_cast<int>(y - camera.y),
        SHIP_SIZE, SHIP_SIZE
    };

    SDL_RenderCopyEx(renderer, texture, &srcRect, &drawRect, angle, NULL, SDL_FLIP_NONE);
}

void Enemy::update(float deltaTime, Player &player) {
    float dirX = player.x - x;
    float dirY = player.y - y;
    float len = std::sqrt(dirX * dirX + dirY * dirY);
    if (len != 0) {
        dirX /= len;
        dirY /= len;
    }

    vx = dirX * speed;
    vy = dirY * speed;

    x += vx * deltaTime;
    y += vy * deltaTime;

    bullets.update(deltaTime);
}

void EnemyManager::init(SDL_Texture* texture) {
    enemyTexture = texture;
}

bool EnemyManager::shootON() {
    return shootTimer >= shootCooldown;
}

bool EnemyManager::spawnON() {
    return spawnTimer >= spawnCooldown;
}

void EnemyManager::resetShootTimer() {
    shootTimer = 0.0f;
}

void EnemyManager::resetSpawnTimer() {
    spawnTimer = 0.0f;
}

void EnemyManager::spawn(float x, float y, SDL_Texture* texture, SDL_Texture* bullet, Player &player) {
    if (spawnON()) {
        SDL_Rect dest = { x, y, SHIP_SIZE, SHIP_SIZE };

        enemies.emplace_back(x, y, texture, dest, bullet);

        resetSpawnTimer();
    }
}

void EnemyManager::render(SDL_Renderer* renderer, Camera &camera) {
    for (auto& e : enemies) {
            e.render(renderer, enemyTexture, camera);
            e.bullets.render(renderer, camera);
    }
}

void EnemyManager::update(float deltaTime, Player &player) {
    for (auto& e : enemies) {
        e.update(deltaTime, player);

        if (shootON()) {
            float dirX = player.x - (e.x + SHIP_SIZE / 2);
            float dirY = player.y - (e.y + SHIP_SIZE / 2) ;

            float spawnX = e.x + SHIP_SIZE / 2 - BULLET_SIZE / 2;
            float spawnY = e.y + SHIP_SIZE / 2 - BULLET_SIZE / 2;

            float len = std::sqrt(dirX * dirX + dirY * dirY);
            if (len != 0) {
            dirX /= len;
            dirY /= len;

            float angle = atan2(player.y - spawnY - BULLET_SIZE / 2, player.x - spawnX - BULLET_SIZE / 2) * 180 / M_PI + 90;

            e.bullets.shoot(spawnX, spawnY, dirX, dirY, e.bullets.bulletSpeed, e.bulletSrcRect, angle);
            }

            resetShootTimer();
        }
    }
    shootTimer += deltaTime;
    spawnTimer += deltaTime;
//    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
//                                     [](const Enemy& e) { return !b.active; }), enemies.end());
}
