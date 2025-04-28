#include "collision.h"

bool Collision::checkCollision(const Vector2D& posA, const Vector2D& sizeA, const Vector2D& posB, const Vector2D& sizeB) const {
    return (
        posA.x < posB.x + sizeB.x &&
        posA.x + sizeA.x > posB.x &&
        posA.y < posB.y + sizeB.y &&
        posA.y + sizeA.y > posB.y
    );
}

bool Collision::obbCollision(const Vector2D& posA, int widthA, int heightA, float angleA,
                              const Vector2D& posB, int widthB, int heightB, float angleB) const
{
    auto getCorners = [](const Vector2D& pos, int w, int h, float angle, Vector2D corners[4]) {
        float rad = angle * M_PI / 180.0f;
        float cosA = cos(rad);
        float sinA = sin(rad);

        Vector2D center = { pos.x + w / 2.0f, pos.y + h / 2.0f };

        Vector2D points[4] = {
            { -w / 2.0f, -h / 2.0f },
            {  w / 2.0f, -h / 2.0f },
            {  w / 2.0f,  h / 2.0f },
            { -w / 2.0f,  h / 2.0f }
        };

        for (int i = 0; i < 4; ++i) {
            float rotatedX = points[i].x * cosA - points[i].y * sinA;
            float rotatedY = points[i].x * sinA + points[i].y * cosA;
            corners[i] = { center.x + rotatedX, center.y + rotatedY };
        }
    };

    auto project = [](const Vector2D corners[4], const Vector2D& axis, float& min, float& max) {
        min = max = (corners[0].x * axis.x + corners[0].y * axis.y);
        for (int i = 1; i < 4; ++i) {
            float proj = (corners[i].x * axis.x + corners[i].y * axis.y);
            if (proj < min) min = proj;
            if (proj > max) max = proj;
        }
    };

    auto overlap = [](float minA, float maxA, float minB, float maxB) {
        return !(maxA < minB || maxB < minA);
    };

    Vector2D cornersA[4], cornersB[4];
    getCorners(posA, widthA, heightA, angleA, cornersA);
    getCorners(posB, widthB, heightB, angleB, cornersB);

    Vector2D axes[4] = {
        { cornersA[1].x - cornersA[0].x, cornersA[1].y - cornersA[0].y },
        { cornersA[3].x - cornersA[0].x, cornersA[3].y - cornersA[0].y },
        { cornersB[1].x - cornersB[0].x, cornersB[1].y - cornersB[0].y },
        { cornersB[3].x - cornersB[0].x, cornersB[3].y - cornersB[0].y }
    };

    for (int i = 0; i < 4; ++i) {
        float length = sqrt(axes[i].x * axes[i].x + axes[i].y * axes[i].y);
        if (length != 0.0f) {
            axes[i].x /= length;
            axes[i].y /= length;
        }
    }

    for (int i = 0; i < 4; ++i) {
        float minA, maxA, minB, maxB;
        project(cornersA, axes[i], minA, maxA);
        project(cornersB, axes[i], minB, maxB);
        if (!overlap(minA, maxA, minB, maxB)) {
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
    if (beam.shooter == bulletFrom::ENEMY) {
        if (obbCollision(player.position, SHIP_SIZE, SHIP_SIZE, 0.0f,
                            beam.position, beam.width, beam.height, beam.angle) )
                            return true;
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
            player.health.takeDamage(e.explosionDamage);
            player.SFX->playSound("hit");
        }

        for (auto& b : e.bullets.bullets) {
            if (!b.active) continue;
            if (bulletXPlayer(player, b)) {
            b.active = false;
            player.health.takeDamage(e.damage);
            player.SFX->playSound("hit");
            player.hurtTimer = player.hurtDuration;
            }
        }

        for (auto& b : e.beams.beams) {
            if (!b.active) continue;
            if (beamXPlayer(player, b)) {
                if (player.beamDamageTimer <= 0.0f) {
                player.health.takeDamage(e.damage);
                player.SFX->playSound("hit");
                player.hurtTimer = player.hurtDuration;
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
                player.SFX->playSound("hit");
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
            player.health.takeDamage(a.damage);
            player.SFX->playSound("hit");
            player.hurtTimer = player.hurtDuration;
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
