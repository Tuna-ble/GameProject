#include "enemy.h"

// ==== Enemy ====

Enemy::Enemy (Vector2D position, SDL_Texture* texture, SDL_Rect dest, SDL_Texture* bullet, SDL_Texture* thrusterTexture, Audio* sound, enemyType eType)
    : position(position), texture(texture), dest(dest), alive(true), health(ENEMY_HEALTH), type(eType) {
        thruster.init(thrusterTexture, SHIP_FRAMES, SHIP_CLIPS);

        switch (type) {
            case enemyType::BEAM: {
                beams.init(bullet);

                auto [x, y] = beamShipTypes[rand() % 2];
                srcRect = { x * 128, y * 128, 128, 128 };
                beamSrcRect = { 2 * 500, 0 * 500, 500, 500 };
                shootCooldown = 5.0f;
                break;
            }
            case enemyType::BULLET: {
                bullets.init(bullet);

                auto [x, y] = bulletShipTypes[rand() % 4];
                srcRect = { x * 128, y * 128, 128, 128 };
                bulletSrcRect = { (enemyBulletID % 3) * 500, (enemyBulletID / 2) * 500, 500, 500 };
                shootCooldown = (float)(1 + rand() % 3);
                break;
            }
        }

        SFX = sound;
        speed = rand() % 150;
        shootTimer = (float)(rand() % 1000) / 1000.0f;
    }

void Enemy::render(SDL_Renderer* renderer, Camera &camera) {
    if (!alive && !dropped) return;

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

    healthBar.render(renderer, health, position - camera.position, 75, 20);

    for (const auto& beam : beams.beams) {
        beam.drawOBB(renderer, camera, { 0, 255, 0, 255 });
    }
}

void Enemy::update(float deltaTime, Graphics& graphics, Player &player, DropManager& drops) {
    Vector2D direction = player.position - position;

    direction = direction.normalize();

    velocity = direction * speed;

    position += velocity * deltaTime;

    shootTimer += deltaTime;

    if (shootON()) {
            float angle = 0.0f;
            switch (type) {
                case enemyType::BEAM: {
                    angle = atan2(player.position.y - position.y, player.position.x - position.x) * 180 / M_PI + 90;
                    beams.shoot(position, direction, damage, beamSrcRect, angle, bulletFrom::ENEMY);
                    break;
                }
                case enemyType::BULLET: {
                    Vector2D spawn = bullets.getBulletSpawnPosition(position);

                    angle = atan2(player.position.y - spawn.y - BULLET_SIZE / 2, player.position.x - spawn.x - BULLET_SIZE / 2) * 180 / M_PI + 90;

                    int bulletSpeed = 40 + rand() % bullets.bulletSpeed;
                    bullets.shoot(spawn, direction, damage, bulletSpeed, bulletSrcRect, angle, bulletFrom::ENEMY);
                    SFX->playSound("shoot");
                    break;
                }
            }

            resetShootTimer();
            }

    if (hurtTimer > 0.0f) {
        hurtTimer -= deltaTime;
    }

    if (health.isDead() && !dropped) {
        if (rand() % 100 > 70) drops.spawn(position);
        dropped = true;
    }

    thruster.update();

    bullets.update(deltaTime);
    beams.update(deltaTime);
    drops.update(deltaTime);
}

bool Enemy::shootON() {
    return shootTimer >= shootCooldown;
}

void Enemy::resetShootTimer() {
    shootTimer = 0.0f;
}

// ==== Enemy Manager ====

EnemyManager::EnemyManager(gameMode& m) : mode(m) {}

void EnemyManager::init(Graphics& graphics, Audio& sound) {
    enemyTexture = graphics.getTexture("spaceShip");
    thrusterTexture = graphics.getTexture("thruster");
    bulletTexture = graphics.getTexture("bullet");
    SFX = &sound;
    spawnCooldown = (float)(1 + rand() % 2);
}

bool EnemyManager::spawnON() {
    return spawnTimer >= spawnCooldown;
}

void EnemyManager::resetSpawnTimer() {
    spawnTimer = 0.0f;
}

Vector2D EnemyManager::spawnEnemyOutsideCamera(Camera& camera, int margin) {
    float x, y;
    int side = rand() % 4;

    switch (side) {
        case 0:
            x = camera.position.x + rand() % SCREEN_WIDTH;
            y = camera.position.y - margin;
            break;
        case 1:
            x = camera.position.x + rand() % SCREEN_WIDTH;
            y = camera.position.y + SCREEN_HEIGHT + margin;
            break;
        case 2:
            x = camera.position.x - margin;
            y = camera.position.y + rand() % SCREEN_HEIGHT;
            break;
        case 3:
            x = camera.position.x + SCREEN_WIDTH + margin;
            y = camera.position.y + rand() % SCREEN_HEIGHT;
            break;
    }

    return Vector2D(x, y);
}

void EnemyManager::spawn(Camera& camera) {
    if (spawnON()) {
        Vector2D spawn = spawnEnemyOutsideCamera(camera, 200);
        SDL_Rect dest = { spawn.x, spawn.y, ENEMY_SIZE, ENEMY_SIZE };

        if (mode == gameMode::NORMAL) {
            scorePerEnemy = 10;
            enemies.emplace_back(spawn, enemyTexture, dest, bulletTexture, thrusterTexture, SFX, enemyType::BULLET);
        }
        else if (mode == gameMode::HARD) {
            scorePerEnemy = 30;
            int r = rand() % 100;
            if (r < 20) {
                type = enemyType::BEAM;
            }
            else {
                type = enemyType::BULLET;
            }

            enemies.emplace_back(spawn, enemyTexture, dest, bulletTexture, thrusterTexture, SFX, type);
        }
        resetSpawnTimer();
    }
}

void EnemyManager::render(SDL_Renderer* renderer, Camera &camera) {
    for (auto& e : enemies) {
            e.render(renderer, camera);
            e.bullets.render(renderer, camera);
            e.beams.render(renderer, camera);
    }
}

void EnemyManager::update(float deltaTime, Graphics& graphics, Player &player, DropManager& drops, ExplosionManager& explosionManager) {
    for (auto& e : enemies) {
        e.update(deltaTime, graphics, player, drops);
        if (!e.alive) {
            deadCount++;
            getScore+=scorePerEnemy;
            if (!e.exploded) {
                explosionManager.spawn(e.position, explodeType::SHIP);
                e.exploded = true;
            }
        }
    }
    spawnTimer += deltaTime;
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                     [](const Enemy& e) { return !e.alive; }), enemies.end());
}

void EnemyManager::reset() {
    enemies.clear();
    spawnTimer = 0;
    deadCount = 0;
    getScore = 0;
}
