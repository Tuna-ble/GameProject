#ifndef HEALTH_H_INCLUDED
#define HEALTH_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "vector2D.h"
#include "audio.h"

struct Health {
    int cap, current;

    Health();
    Health(int maxHealth);
    void takeDamage(int damage);

    void heal(int amount);

    bool isDead() const;

    float getPercent() const;

    void renderHealthBar(SDL_Renderer* renderer, Vector2D position, int width, int height, float percent);
};

#endif // HEALTH_H_INCLUDED
