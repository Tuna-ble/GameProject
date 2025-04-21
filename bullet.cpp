#include "player.h"
#include "bullet.h"

// ==== Bullet ====

Bullet::Bullet(Vector2D position, Vector2D velocity, int damage, SDL_Texture* texture, SDL_Rect src, float angle, bulletFrom shooter)
    : position(position), velocity(velocity), damage(damage),
      width(BULLET_SIZE), height(BULLET_SIZE),
      active(true), texture(texture), srcRect(src), angle(angle), shooter(shooter) {}

void Bullet::update(float deltaTime) {
    position += velocity * deltaTime;
    activeTimer += deltaTime;

    if (activeTimer >= activeDuration) {
        active = false;
    }
}

void Bullet::render(SDL_Renderer* renderer, Camera& camera) {
    if (!active) return;
    SDL_Rect dst = {(int)(position.x - camera.position.x), (int)(position.y - camera.position.y), width, height};
    SDL_RenderCopyEx(renderer, this->texture, &this->srcRect, &dst, angle, NULL, SDL_FLIP_NONE);
}

// ==== Bullet Manager ====

void BulletManager::init(SDL_Texture* texture) {
    bulletTexture = texture;
    }

void BulletManager::shoot(Vector2D position, Vector2D direction, int enemyDamage, float speed, const SDL_Rect& srcRect, float angle, bulletFrom shooter) {

        bullets.emplace_back(position, direction * speed, enemyDamage, bulletTexture, srcRect, angle, shooter);
    }

void BulletManager::update(float deltaTime) {
        for (auto& b : bullets) b.update(deltaTime);
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                     [](const Bullet& b) { return !b.active; }), bullets.end());
    }

void BulletManager::render(SDL_Renderer* renderer, Camera& camera) {
    for (auto& b : bullets) b.render(renderer, camera);
}

Vector2D BulletManager::getBulletSpawnPosition(Vector2D& position) const {
    return position + Vector2D(SHIP_SIZE / 2.0f - BULLET_SIZE / 2.0f, SHIP_SIZE / 2.0f - BULLET_SIZE / 2.0f);
}
