#ifndef BEAM_H_INCLUDED
#define BEAM_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "background.h"
#include "vector2D.h"
#include "sprite.h"

struct Beam {
    Vector2D position;
    Vector2D direction;
    Sprite beamShooting;
    int width = SHIP_SIZE;
    int height = SCREEN_WIDTH + 200;
    bool active;
    float angle;
    int damage;
    bulletFrom shooter;

    SDL_Texture* beamTexture;
    SDL_Rect beamRect;

    float activeTimer = 0.0f;
    float activeDuration = 2.0f;

    Beam(Vector2D position, Vector2D direction, int damage, SDL_Texture* tex, SDL_Rect src, float angle, bulletFrom shooter);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, Camera& camera);
};

struct BeamManager {
    std::vector<Beam> beams;
    SDL_Texture* beamTexture;

    void init(SDL_Texture* texture);

    void shoot(Vector2D position, Vector2D direction, int damage, const SDL_Rect& srcRect, float angle, bulletFrom shooter);

    void update(float deltaTime);

    void render(SDL_Renderer* renderer, Camera& camera);

    Vector2D getBeamSpawnPosition(Vector2D& position) const;
};

#endif // BEAM_H_INCLUDED
