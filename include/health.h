#ifndef HEALTH_H_INCLUDED
#define HEALTH_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "vector2D.h"
#include "audio.h"
#include "healthOwner.h"

struct Health {
    float cap, current;

    Health();
    Health(float maxHealth);
    void takeDamage(int damage);

    void heal(float amount);

    bool isDead() const;

    float getPercent() const;

    void renderHealthBar(SDL_Renderer* renderer, SDL_Texture* barTexture, SDL_Texture* fillTexture, SDL_Rect barRect);
};

struct HealthBar {
    static SDL_Texture* barLineTexture;
    static SDL_Texture* barFillTexture;
    static SDL_Texture* fillTexture;

    static SDL_Texture* barTexture;
    static SDL_Texture* eFillTexture;

    SDL_Rect healthBarRect;
    SDL_Rect healthFillRect;

    static void setTextures(SDL_Texture* bar, SDL_Texture* line, SDL_Texture* barFill, SDL_Texture* fill, SDL_Texture* fill1);
    void render(SDL_Renderer* renderer, Health& health, Vector2D position, int width, int height, healthOwner owner);
    void cleanUp();
};

#endif // HEALTH_H_INCLUDED
