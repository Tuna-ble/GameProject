#include "health.h"

SDL_Texture* HealthBar::barTexture;
SDL_Texture* HealthBar::eFillTexture;

SDL_Texture* HealthBar::barLineTexture;
SDL_Texture* HealthBar::barFillTexture;
SDL_Texture* HealthBar::fillTexture;

// ==== Health ====

Health::Health() = default;

Health::Health(float maxHealth) : cap(maxHealth), current(maxHealth) {}

void Health::takeDamage(int damage) {
    current -= damage;
    if (current < 0) current = 0;
}

void Health::heal(float amount) {
    current += amount;
    if (current > cap) current = cap;
}

bool Health::isDead() const {
    return current <= 0;
}

float Health::getPercent() const {
    return cap > 0 ? current / cap : 0.0f;
}

// ==== Health Bar ====

void HealthBar::setTextures(SDL_Texture* bar, SDL_Texture* line, SDL_Texture* barFill, SDL_Texture* fill, SDL_Texture* fill1) {
    barLineTexture = line;
    barFillTexture = barFill;
    fillTexture = fill;

    barTexture = bar;
    eFillTexture = fill1;
}

void HealthBar::render(SDL_Renderer* renderer, Health& health, Vector2D position, int width, int height, healthOwner owner) {
    healthBarRect = {
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        width, height
    };
    healthFillRect = {
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        static_cast<int>(width * health.getPercent()),
        height
    };

    if (owner == healthOwner::PLAYER) {
        SDL_RenderCopy(renderer, barFillTexture, nullptr, &healthBarRect);

        SDL_RenderCopy(renderer, fillTexture, nullptr, &healthFillRect);

        SDL_RenderCopy(renderer, barLineTexture, nullptr, &healthBarRect);
    }

    else if (owner == healthOwner::ENEMY) {
        SDL_RenderCopy(renderer, barTexture, nullptr, &healthBarRect);

        float percent = health.getPercent();
        SDL_SetTextureColorMod(eFillTexture, 255 * (1.0f - percent), 255 * percent, 0);
        SDL_RenderCopy(renderer, eFillTexture, nullptr, &healthFillRect);
    }
}

void HealthBar::cleanUp() {
    SDL_DestroyTexture(barLineTexture);
    SDL_DestroyTexture(barFillTexture);
    SDL_DestroyTexture(fillTexture);
}
