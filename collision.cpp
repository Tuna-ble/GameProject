#include "collision.h"

bool Collision::checkCollision(const Vector2D& posA, const Vector2D& sizeA, const Vector2D& posB, const Vector2D& sizeB) const {
    return (
        posA.x < posB.x + sizeB.x &&
        posA.x + sizeA.x > posB.x &&
        posA.y < posB.y + sizeB.y &&
        posA.y + sizeA.y > posB.y
    );
}

void Collision::getCorners(Vector2D corners[4], Vector2D position, int width, int height, float angle) const {
    float rad = angle * M_PI / 180.0f;
    float cosA = cos(rad);
    float sinA = sin(rad);

    Vector2D center = { position.x, position.y };

    Vector2D localCorners[4] = {
        { -width / 2.0f, 0.0f },
        {  width / 2.0f, 0.0f },
        {  width / 2.0f, -height },
        { -width / 2.0f, -height }
    };

    for (int i = 0; i < 4; ++i) {
        float rotatedX = localCorners[i].x * cosA - localCorners[i].y * sinA;
        float rotatedY = localCorners[i].x * sinA + localCorners[i].y * cosA;
        corners[i] = { center.x + rotatedX, center.y + rotatedY };
    }
}


bool OBBCollision(const Vector2D cornersA[4], const Vector2D cornersB[4]) {
    Vector2D axes[4];

    for (int i = 0; i < 2; ++i) {
        axes[i] = Vector2D{
            cornersA[(i + 1) % 4].x - cornersA[i].x,
            cornersA[(i + 1) % 4].y - cornersA[i].y
        }.normalize();
    }
    for (int i = 0; i < 2; ++i) {
        axes[i + 2] = Vector2D{
            cornersB[(i + 1) % 4].x - cornersB[i].x,
            cornersB[(i + 1) % 4].y - cornersB[i].y
        }.normalize();
    }

    for (int i = 0; i < 4; ++i) {
        float minA = INFINITY, maxA = -INFINITY;
        float minB = INFINITY, maxB = -INFINITY;

        for (int j = 0; j < 4; ++j) {
            float projectionA = cornersA[j].x * axes[i].x + cornersA[j].y * axes[i].y;
            minA = std::min(minA, projectionA);
            maxA = std::max(maxA, projectionA);

            float projectionB = cornersB[j].x * axes[i].x + cornersB[j].y * axes[i].y;
            minB = std::min(minB, projectionB);
            maxB = std::max(maxB, projectionB);
        }

        if (maxA < minB || maxB < minA) {
            return false;
        }
    }

    return true;
}

bool Collision::bulletXEnemy(Enemy& enemy, Bullet& bullet) {
    if (bullet.shooter == bulletFrom::PLAYER) {
        if (checkCollision(enemy.position, {ENEMY_SIZE, ENEMY_SIZE}, bullet.position, {BULLET_SIZE, BULLET_SIZE}))
            return true;
    }
    return false;
}

bool Collision::bulletXPlayer(Player& player, Bullet& bullet) {
    if (bullet.shooter == bulletFrom::ENEMY) {
        if (checkCollision(player.position, {SHIP_SIZE, SHIP_SIZE}, bullet.position, {BULLET_SIZE, BULLET_SIZE}))
            return true;
    }
    return false;
}

bool Collision::beamXPlayer(Player& player, Beam& beam) {
    if (beam.fired && beam.shooter == bulletFrom::ENEMY) {
        Vector2D beamCorners[4];
        getCorners(beamCorners, beam.position, beam.width, beam.height, beam.angle);

        Vector2D playerCorners[4];
        getCorners(playerCorners, player.position, SHIP_SIZE, SHIP_SIZE, player.angle);

        if (OBBCollision(beamCorners, playerCorners)) {
            return true;
        }
    }
    return false;
}

bool Collision::beamXEnemy(Enemy& enemy, Beam& beam) {
    if (beam.fired && beam.shooter == bulletFrom::PLAYER) {
        Vector2D beamCorners[4];
        getCorners(beamCorners, beam.position, beam.width, beam.height, beam.angle);

        Vector2D enemyCorners[4];
        getCorners(enemyCorners, enemy.position, ENEMY_SIZE, ENEMY_SIZE, enemy.angle);

        if (OBBCollision(beamCorners, enemyCorners)) {
            return true;
        }
    }
    return false;
}

bool Collision::enemyXPlayer(Player& player, Enemy& enemy) {
    if (checkCollision(enemy.position, {ENEMY_SIZE, ENEMY_SIZE}, player.position, {SHIP_SIZE, SHIP_SIZE}))
            return true;
    return false;
}

bool Collision::asteroidXPlayer(Player& player, Asteroid& asteroid) {
    if (checkCollision(asteroid.position, {ASTEROID_SIZE, ASTEROID_SIZE}, player.position, {SHIP_SIZE, SHIP_SIZE}))
        return true;
    return false;
}

bool Collision::dropXPlayer(Player& player, Drop& drop) {
    if (checkCollision(drop.position, {DROP_SIZE, DROP_SIZE}, player.position, {SHIP_SIZE, SHIP_SIZE}))
        return true;
    return false;
}

void Collision::checkAll(std::vector<Enemy>& enemies, std::vector<Asteroid>& asteroids, std::vector<Drop>& drops, Player& player) {
    for (auto& e : enemies) {
        if (!e.alive) continue;
        if (enemyXPlayer(player, e)) {
            e.alive = false;
            e.SFX->playSound("explosion");

            if (!player.shield.isShielding()) {
                player.health.takeDamage(e.explosionDamage);
                player.hurtTimer = player.hurtDuration;
            }
            player.SFX->playSound("hit");
        }

        for (auto& b : e.bullets.bullets) {
            if (!b.active) continue;
            if (bulletXPlayer(player, b)) {
            b.active = false;

            if (!player.shield.isShielding()) {
                player.health.takeDamage(e.damage);
                player.hurtTimer = player.hurtDuration;
            }
            player.SFX->playSound("hit");
            }
        }

        for (auto& b : e.beams.beams) {
            if (!b.active) continue;
            if (beamXPlayer(player, b)) {
                if (player.beamDamageTimer <= 0.0f) {

                if (!player.shield.isShielding()) {
                    player.health.takeDamage(e.beamDamage);
                    player.hurtTimer = player.hurtDuration;
                }
                player.SFX->playSound("hit");
                player.beamDamageTimer = player.beamDamageInterval;
                }
            }
        }
    }

    for (auto& b : player.bullets.bullets) {
        if (!b.active) continue;

        for (auto& e : enemies) {
            if (!e.alive) continue;

            if (bulletXEnemy(e, b)) {
                b.active = false;
                e.health.takeDamage(player.damage);
                e.SFX->playSound("hit");
                e.hurtTimer = e.hurtDuration;
                if (e.health.isDead()) {
                    e.alive = false;
                    e.SFX->playSound("explosion");
                }
            }
        }
    }

    for (auto& b : player.beams.beams) {
            if (!b.active) continue;
            for (auto& e : enemies) {
                if (!e.alive) continue;

                if (beamXEnemy(e, b)) {
                    e.health.takeDamage(player.damage);
                    e.SFX->playSound("hit");
                    e.hurtTimer = e.hurtDuration;
                    if (e.health.isDead()) {
                        e.alive = false;
                        e.SFX->playSound("explosion");
                    }
                }
            }
        }

    for (auto& a : asteroids) {
        if (!a.active) continue;

        if (asteroidXPlayer(player, a)) {
            a.active = false;
            if (!player.shield.isShielding()) {
                player.health.takeDamage(a.damage);
                player.hurtTimer = player.hurtDuration;
            }
            player.SFX->playSound("hit");
            a.SFX->playSound("explosion");
        }
    }

    for (auto& d : drops) {
        if (!d.active) continue;

        if (dropXPlayer(player, d)) {
            d.active = false;
            player.SFX->playSound("getBuff");
            player.getBuff(d.buffValue, d.type);
        }
    }
}
