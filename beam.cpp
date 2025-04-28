#include "beam.h"

// ==== Beam ====

Beam::Beam(Vector2D position, Vector2D direction, int damage, SDL_Texture* tex, SDL_Rect src, float angle, bulletFrom shooter)
    : position(position), direction(direction), damage(damage),
    beamTexture(tex), beamRect(src), angle(angle), shooter(shooter), active(true) {}

void Beam::update(float deltaTime) {
    activeTimer += deltaTime;
    if (activeTimer >= activeDuration) {
        active = false;
    }
}

void Beam::render(SDL_Renderer* renderer, Camera& camera) {
    if (!active) return;

    SDL_Point center = { width / 2, height };

    SDL_Rect dst = {
        static_cast<int>(position.x - camera.position.x),
        static_cast<int>(position.y - camera.position.y - height),
        width,
        height
    };

    SDL_RenderCopyEx(renderer, beamTexture, &beamRect, &dst, angle, &center, SDL_FLIP_NONE);
}


// ==== Beam Manager ====

void BeamManager::init(SDL_Texture* texture) {
    beamTexture = texture;
}

void BeamManager::shoot(Vector2D position, Vector2D direction, int damage, const SDL_Rect& srcRect, float angle, bulletFrom shooter) {
    beams.emplace_back(position, direction, damage, beamTexture, srcRect, angle, shooter);
}

void BeamManager::update(float deltaTime) {
    for (auto& b : beams) b.update(deltaTime);
    beams.erase(std::remove_if(beams.begin(), beams.end(),
                                 [](const Beam& b) { return !b.active; }), beams.end());
}

void BeamManager::render(SDL_Renderer* renderer, Camera& camera) {
    for (auto& b : beams) b.render(renderer, camera);
}

Vector2D BeamManager::getBeamSpawnPosition(Vector2D& position) const {
    return position + Vector2D(SHIP_SIZE / 2.0f - SHIP_SIZE / 2.0f, SHIP_SIZE / 2.0f);
}
