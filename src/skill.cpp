#include "skill.h"

void Shield::init(SDL_Texture* texture) {
    shieldTexture = texture;
    activeTime = 0.0f;
    cooldownTime = cooldown;
    active = false;
}

void Shield::activate() {
    if (!active && cooldownTime >= cooldown) {
        active = true;
        activeTime = 0;
        cooldownTime = 0;
    }
}

void Shield::update(float deltaTime) {
    if (active) {
        activeTime += deltaTime;
        if (activeTime >= duration) {
            active = false;
        }
    }
    else {
        cooldownTime += deltaTime;
    }
}

bool Shield::isShielding() const {
    return active;
}

bool Shield::canActivate() const {
    return !active && cooldownTime >= cooldown;
}

float Shield::getCDPercent() const {
    return cooldownTime <= cooldown ? cooldownTime / cooldown : 1.0f;
}

void Shield::reset() {
    active = false;
    activeTime = 0;
    cooldownTime = cooldown;
}

void Shield::render(SDL_Renderer* renderer, const SDL_Rect& playerRect) {
    if (isShielding())
        SDL_RenderCopy(renderer, shieldTexture, NULL, &playerRect);
}
