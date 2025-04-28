#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "asteroid.h"
#include "vector2D.h"

struct Collision {
    bool checkCollision(const Vector2D& posA, const Vector2D& sizeA, const Vector2D& posB, const Vector2D& sizeB) const;
    bool obbCollision(const Vector2D& posA, int widthA, int heightA, float angleA,
                              const Vector2D& posB, int widthB, int heightB, float angleB) const;
    bool bulletXEnemy(Enemy& enemy, Bullet& bullet);
    bool bulletXPlayer(Player& player, Bullet& bullet);
    bool beamXPlayer(Player& player, Beam& beam);
    bool enemyXPlayer(Player& player, Enemy& enemy);
    bool asteroidXPlayer(Player& player, Asteroid& asteroid);
    bool dropXPlayer(Player& player, Drop& drop);
    void checkAll(std::vector<Enemy>& enemies, std::vector<Asteroid>& asteroids, std::vector<Drop>& drops, Player& player);
};

#endif // COLLISION_H_INCLUDED
