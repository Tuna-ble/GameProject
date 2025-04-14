#include "collision.h"

bool Collision::checkCollision(const Vector2D& posA, const Vector2D& sizeA, const Vector2D& posB, const Vector2D& sizeB) const {
        Vector2D halfA = sizeA * 0.5f;
        Vector2D halfB = sizeB * 0.5f;

        return (abs(posA.x - posB.x) < (halfA.x + halfB.x) && abs(posA.y - posB.y) < (halfA.y + halfB.y));
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

void Collision::checkAll(std::vector<Enemy>& enemies, Player& player) {
    for (auto& e : enemies) {
        if (!e.alive) continue;
        if (enemyXPlayer(player, e)) {
            e.alive = false;
            player.health.takeDamage(e.damage);
            std::cerr << "You are caught in explosion" << "\n";
            std::cerr << "Health :" << player.health.getPercent() << "\n";
        }
        for (auto& b : e.bullets.bullets) {
            if (!b.active) continue;
            if (bulletXPlayer(player, b)) {
            b.active = false;
            player.health.takeDamage(e.damage);
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
                e.hurtTimer = e.hurtDuration;
                if (e.health.isDead()) e.alive = false;
            }
        }
    }
}
