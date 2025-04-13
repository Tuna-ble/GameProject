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
