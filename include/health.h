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

    void renderHealthBar(SDL_Renderer* renderer, SDL_Texture* barTexture, SDL_Texture* fillTexture, SDL_Rect barRect);
};

struct HealthBar {
    static SDL_Texture* barTexture;
    static SDL_Texture* fillTexture;

    SDL_Rect healthBarRect;
    SDL_Rect healthFillRect;

    static void setTextures(SDL_Texture* bar, SDL_Texture* fill);
    void render(SDL_Renderer* renderer, Health& health, Vector2D position, int width, int height);
    void cleanUp();
};

#endif // HEALTH_H_INCLUDED
