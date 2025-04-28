#ifndef ASTEROID_H_INCLUDED
#define ASTEROID_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "vector2D.h"
#include "sprite.h"
#include "audio.h"
#include "background.h"
#include "explosion.h"

struct Asteroid {
    Vector2D position;
    Vector2D velocity;
    SDL_Texture* texture;
    SDL_Rect dest;
    int damage = 4;
    bool active;
    bool exploded = false;

    float activeTimer = 0.0f;
    float activeDuration = 15.0f;

    Audio* SFX;
    Sprite explosion;

    Asteroid(Vector2D position, Vector2D velocity, SDL_Texture* texture, SDL_Rect dest, Audio& sound);
    void render(SDL_Renderer* renderer, SDL_Texture* texture, Camera &camera);
    void update(float deltaTime);
};


struct AsteroidManager {
    std::vector <Asteroid> asteroids;
    SDL_Texture* asteroidTexture;
    Audio SFX;

    int speed;
    float spawnTimer = 0.0f;
    float spawnCooldown;

    void init(Graphics& graphics, Audio& sound);

    void resetSpawnTimer();
    bool spawnON();

    Vector2D spawnOutsideCamera(Camera& camera, int margin);
    void spawn(Camera& camera);
    void update(float deltaTime, ExplosionManager& explosionManager);
    void render(SDL_Renderer* renderer, Camera &camera);
    void reset();
};

#endif // ASTEROID_H_INCLUDED
