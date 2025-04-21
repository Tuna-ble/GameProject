#ifndef EXPLOSION_H_INCLUDED
#define EXPLOSION_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "sprite.h"
#include "vector2D.h"
#include "graphics.h"

enum class explodeType {
    SHIP,
    ASTEROID,
};

struct Explosion {
    Vector2D position;
    Sprite explode;
    int explodeSize;

    Explosion(Vector2D pos, SDL_Texture* texture, explodeType type, int size);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, Camera& camera);
    bool isFinished() const;
};


struct ExplosionManager {
    std::vector<Explosion> explosions;
    SDL_Texture* shipExplosionTexture;
    SDL_Texture* asteroidExplosionTexture;

    void init(Graphics& graphics);
    void spawn(Vector2D position, explodeType type);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, Camera& camera);
};


#endif // EXPLOSION_H_INCLUDED
