#include "health.h"

SDL_Texture* HealthBar::barTexture;
SDL_Texture* HealthBar::fillTexture;
// ==== Health ====

Health::Health() = default;

Health::Health(int maxHealth) : cap(maxHealth), current(maxHealth) {}

void Health::takeDamage(int damage) {
    current -= damage;
    if (current < 0) current = 0;
}

void Health::heal(int amount) {
    current += amount;
    if (current > cap) current = cap;
}

bool Health::isDead() const {
    return current <= 0;
}

float Health::getPercent() const {
    return cap > 0 ? (float)current / cap : 0.0f;
}

// ==== Health Bar ====

void HealthBar::render(SDL_Renderer* renderer, Health& health, Vector2D position, int width, int height) {
    healthBarRect = { static_cast<int>(position.x), static_cast<int>(position.y), width, height };
    healthFillRect = { static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(width * health.getPercent()), height };

    SDL_RenderCopy(renderer, barTexture, nullptr, &healthBarRect);

    float percent = health.getPercent();
    SDL_SetTextureColorMod(fillTexture, 255 * (1.0f - percent), 255 * percent, 0);
    SDL_RenderCopy(renderer, fillTexture, nullptr, &healthFillRect);
}

void HealthBar::setTextures(SDL_Texture* bar, SDL_Texture* fill) {
    barTexture = bar;
    fillTexture = fill;
}

void HealthBar::cleanUp() {
    SDL_DestroyTexture(barTexture);
    SDL_DestroyTexture(fillTexture);
}
