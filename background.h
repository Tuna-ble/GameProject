#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED
#include "def.h"
#include "player.h"

struct Player;

struct Camera
{
    float x, y;
    const int w = SCREEN_WIDTH, h = SCREEN_HEIGHT;

    SDL_Rect getViewRect() const;
    void updateCamera(Player &player);
};

struct TiledRenderer
{
    float paralaxStrength = 1.0f;

    void renderer(SDL_Renderer* renderer, SDL_Texture* texture, Camera& camera);
};

#endif // BACKGROUND_H_INCLUDED
