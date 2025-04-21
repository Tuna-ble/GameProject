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

//void Health::renderHealthBar(SDL_Renderer* renderer, SDL_Texture* barTexture, SDL_Texture* fillTexture, SDL_Rect barRect) {
//    SDL_RenderCopy(renderer, barTexture, nullptr, &barRect);
//
//    SDL_Rect fillRect = {
//        barRect.x,
//        barRect.y,
//        static_cast<int>(barRect.w * getPercent()),
//        barRect.h
//    };
//    SDL_RenderCopy(renderer, fillTexture, nullptr, &fillRect);
//
////    SDL_Rect border = { (int)position.x, (int)position.y, width, height };
////    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
////    SDL_RenderDrawRect(renderer, &border);
////
////    SDL_Rect fill = { (int)position.x + 1, (int)position.y + 1, (int)((width - 2) * percent), height - 2 };
////    SDL_SetRenderDrawColor(renderer, 255 * (1.0f - percent), 255 * percent, 0, 255);
////    SDL_RenderFillRect(renderer, &fill);
//}
