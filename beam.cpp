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
        static_cast<int>(position.y - camera.position.y - height ),
        width,
        height
    };

    SDL_RenderCopyEx(renderer, beamTexture, &beamRect, &dst, angle, &center, SDL_FLIP_NONE);
}

void Beam::drawOBB(SDL_Renderer* renderer, const Camera& camera, SDL_Color color) const {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    Vector2D corners[4];
    float rad = angle * M_PI / 180.0f;
    float cosA = cos(rad);
    float sinA = sin(rad);

    Vector2D center = { position.x + width / 2, position.y };

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

    for (int i = 0; i < 4; ++i) {
        SDL_RenderDrawLineF(renderer,
            corners[i].x - camera.position.x, corners[i].y - camera.position.y,
            corners[(i + 1) % 4].x - camera.position.x, corners[(i + 1) % 4].y - camera.position.y
        );
    }
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
