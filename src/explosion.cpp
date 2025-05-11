#include "explosion.h"
#include "background.h"

// ==== Explosion ====

Explosion::Explosion(Vector2D pos, SDL_Texture* texture, explodeType type, int size) : position(pos), explodeSize(size) {
    switch (type) {
        case explodeType::SHIP:
            explode.init(texture, EXPLOSION_FRAMES, EXPLOSION_CLIPS);
            break;
        case explodeType::ASTEROID:
            explode.init(texture, ASEXPLOSION_FRAMES, ASEXPLOSION_CLIPS);
            break;
    }
    explode.loop = false;
    explode.currentFrame = 0;
    explode.elapsedTime = 0.0f;
    explode.finished = false;
}

void Explosion::update(float deltaTime) {
    explode.animate(deltaTime);
}

void Explosion::render(SDL_Renderer* renderer, Camera& camera) {
    Vector2D draw = position - camera.position;
    SDL_Rect drawRect = {
        static_cast<int>(draw.x),
        static_cast<int>(draw.y),
        explodeSize, explodeSize
    };

    explode.render(renderer, position, camera, drawRect, 0, NULL);
}

bool Explosion::isFinished() const {
    return explode.isFinished();
}

// ==== Explosion Manager ====

void ExplosionManager::init(Graphics& graphics) {
        shipExplosionTexture = graphics.getTexture("explosion");
        asteroidExplosionTexture = graphics.getTexture("asteroid_explode");
    }

void ExplosionManager::spawn(Vector2D position, explodeType type) {
    switch (type) {
        case explodeType::SHIP:
            explosions.emplace_back(position, shipExplosionTexture, type, ENEMY_SIZE);
            break;
        case explodeType::ASTEROID:
            explosions.emplace_back(position, asteroidExplosionTexture, type, ASTEROID_SIZE * 2.5);
            break;
    }
}

void ExplosionManager::update(float deltaTime) {
    for (auto& e : explosions) {
        e.update(deltaTime);
    }

    explosions.erase(
        std::remove_if(explosions.begin(), explosions.end(),
            [](Explosion& e) { return e.isFinished(); }),
        explosions.end()
    );
}

void ExplosionManager::render(SDL_Renderer* renderer, Camera& camera) {
    for (auto& e : explosions) {
        e.render(renderer, camera);
    }
}
