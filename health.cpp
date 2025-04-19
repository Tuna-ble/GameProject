#include "health.h"

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

void Health::renderHealthBar(SDL_Renderer* renderer, Vector2D position, int width, int height, float percent) {
    SDL_Rect border = { (int)position.x, (int)position.y, width, height };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &border);

    SDL_Rect fill = { (int)position.x + 1, (int)position.y + 1, (int)((width - 2) * percent), height - 2 };
    SDL_SetRenderDrawColor(renderer, 255 * (1.0f - percent), 255 * percent, 0, 255);
    SDL_RenderFillRect(renderer, &fill);
}
