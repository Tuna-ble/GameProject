#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "vector2D.h"

struct Collision {
    bool checkCollision(const Vector2D& posA, const Vector2D& sizeA, const Vector2D& posB, const Vector2D& sizeB) const;
    bool bulletXEnemy(Enemy& enemy, Bullet& bullet);
    bool bulletXPlayer(Player& player, Bullet& bullet);
    bool enemyXPlayer(Player& player, Enemy& enemy);
    void checkAll(std::vector<Enemy>& enemies, Player& player);
};

#endif // COLLISION_H_INCLUDED
