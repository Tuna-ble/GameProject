#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED
#include "def.h"
#include "player.h"
#include "vector2d.h"

struct Player;

struct Camera
{
    Vector2D position;
    const int w = SCREEN_WIDTH, h = SCREEN_HEIGHT;

    SDL_Rect getViewRect() const;
    void update(Player &player);
};

struct TiledRenderer
{
    float parallaxStrength = 1.0f;
    int tileWidth;
    int tileHeight;

    void render(SDL_Renderer* renderer, SDL_Texture* texture, Camera& camera);
};

#endif // BACKGROUND_H_INCLUDED
