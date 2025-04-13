#ifndef HEALTH_H_INCLUDED
#define HEALTH_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>

struct Health {
    int cap, current;

    Health();
    Health(int maxHealth);
    void takeDamage(int damage);

    void heal(int amount);

    bool isDead() const;

    float getPercent() const;
};

#endif // HEALTH_H_INCLUDED
