#include "drop.h"

// ==== Drop ====

Drop::Drop(Vector2D position, SDL_Texture* texture, SDL_Rect dest, int ID) :
    position(position), texture(texture), dest(dest), active(true) {
        srcRect = {(ID % 2) * 16, (ID / 2) * 16, 16, 16};
        type = static_cast<dropType>(ID);

        switch (type) {
        case dropType::HEALTH:
            buffValue = 4;
            break;
        case dropType::DAMAGE:
            buffValue = 2;
            break;
        case dropType::SPEED:
            buffValue = 20;
            break;
        }
    }

void Drop::update(float deltaTime) {
    activeTimer += deltaTime;

    if (activeTimer >= activeDuration) {
        active = false;
    }
}

void Drop::render(SDL_Renderer* renderer, const Camera& camera) {
    if (!active) {
        return;
    }

    dest.x = static_cast<int>(position.x - camera.position.x);
    dest.y = static_cast<int>(position.y - camera.position.y);

    SDL_RenderCopy(renderer, texture, &srcRect, &dest);
}

// ==== Drop Manager ====

void DropManager::init(Graphics& graphics) {
    dropTexture = graphics.getTexture("power-up");
}

void DropManager::spawn(Vector2D position) {
    int ID = rand() % 3;

    SDL_Rect dest = { static_cast<int>(position.x), static_cast<int>(position.y), DROP_SIZE, DROP_SIZE };
    drops.emplace_back(position, dropTexture, dest, ID);
}
void DropManager::update(float deltaTime) {
    for (auto& d : drops) {
        d.update(deltaTime);
    }
    drops.erase(std::remove_if(drops.begin(), drops.end(),
                                     [](const Drop& a) { return !a.active; }), drops.end());
}
void DropManager::render(SDL_Renderer* renderer, const Camera& camera) {
    for (auto& d : drops) {
        d.render(renderer, camera);
    }
}
