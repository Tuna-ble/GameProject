#include "enemy.h"


#define speed 150

Enemy::Enemy (Vector2D position, SDL_Texture* texture, SDL_Rect dest, SDL_Texture* bullet, SDL_Texture* thrusterTexture)
    : position(position), texture(texture), dest(dest), alive(true), health(4) {
        bullets.init(bullet);
        thruster.init(thrusterTexture, THRUSTER_FRAMES, THRUSTER_CLIPS);
        srcRect = { (1 % 2) * 48, (1 / 2) * 48, 48, 48 };
        bulletSrcRect = { (ID % 3) * 500, (ID / 2) * 500, 500, 500 };
    }

void Enemy::render(SDL_Renderer* renderer, SDL_Texture* texture, Camera &camera) {
    if (!alive) return;

    float angle = atan2(velocity.y, velocity.x) * 180 / M_PI + 90;
    Vector2D draw = position - camera.position;

    if (hurtTimer > 0.0f) {
    draw.x += rand() % 5 - 2;
    draw.y += rand() % 5 - 2;
    }

    SDL_Rect drawRect = {
        static_cast<int>(draw.x),
        static_cast<int>(draw.y),
        ENEMY_SIZE, ENEMY_SIZE
    };

    if (hurtTimer > 0.0f)
    SDL_SetTextureColorMod(texture, 255, 100, 100);
    else
    SDL_SetTextureColorMod(texture, 255, 255, 255);

    thruster.render(renderer, position, camera, ENEMY_SIZE, angle);

    SDL_RenderCopyEx(renderer, texture, &srcRect, &drawRect, angle, NULL, SDL_FLIP_NONE);

    health.renderHealthBar(renderer, {draw.x, draw.y - 6}, ENEMY_SIZE, 5, health.getPercent());
}

void Enemy::update(float deltaTime, Player &player) {
    Vector2D direction = player.position - position;

    direction = direction.normalize();

    velocity = direction * speed;

    position += velocity * deltaTime;

    shootTimer += deltaTime;

    if (shootON()) {
            Vector2D spawn = bullets.getBulletSpawnPosition(position);

            float angle = atan2(player.position.y - spawn.y - BULLET_SIZE / 2, player.position.x - spawn.x - BULLET_SIZE / 2) * 180 / M_PI + 90;

            bullets.shoot(spawn, direction, damage, bullets.bulletSpeed, bulletSrcRect, angle, bulletFrom::ENEMY);

            resetShootTimer();
            }

    if (hurtTimer > 0.0f)
    hurtTimer -= deltaTime;

    thruster.update();

    bullets.update(deltaTime);
}

bool Enemy::shootON() {
    return shootTimer >= shootCooldown;
}

void Enemy::resetShootTimer() {
    shootTimer = 0.0f;
}

void EnemyManager::init(SDL_Texture* texture) {
    enemyTexture = texture;
}

bool EnemyManager::spawnON() {
    return spawnTimer >= spawnCooldown;
}

void EnemyManager::resetSpawnTimer() {
    spawnTimer = 0.0f;
}

void EnemyManager::spawn(Vector2D spawn, SDL_Texture* texture, SDL_Texture* bullet, SDL_Texture* thruster, Player &player) {
    if (spawnON()) {
        SDL_Rect dest = { static_cast<float>(spawn.x), static_cast<float>(spawn.y), ENEMY_SIZE, ENEMY_SIZE };

        enemies.emplace_back(spawn, texture, dest, bullet, thruster);

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

        }
    spawnTimer += deltaTime;
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                     [](const Enemy& e) { return !e.alive; }), enemies.end());
}
