#include "collision.h"

bool Collision::checkCollision(const Vector2D& posA, const Vector2D& sizeA, const Vector2D& posB, const Vector2D& sizeB) const {
    return (
        posA.x < posB.x + sizeB.x &&
        posA.x + sizeA.x > posB.x &&
        posA.y < posB.y + sizeB.y &&
        posA.y + sizeA.y > posB.y
    );
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
            player.health.takeDamage(e.damage);
            player.SFX->playSound("hit");
            std::cerr << "You are caught in explosion" << "\n";
            std::cerr << "Health :" << player.health.getPercent() << "\n";
        }

        for (auto& b : e.bullets.bullets) {
            if (!b.active) continue;
            if (bulletXPlayer(player, b)) {
            b.active = false;
            player.health.takeDamage(e.damage);
            player.SFX->playSound("hit");
            player.hurtTimer = player.hurtDuration;
            std::cerr << "You are shot" << "\n";
            std::cerr << "Health :" << player.health.getPercent() << "\n";
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
            a.SFX.playSound("explosion");
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
