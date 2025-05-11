#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "player.h"
#include "vector2d.h"
#include "graphics.h"

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
    SDL_Texture* backgroundTexture[6];
    float parallaxStrength[6];
    float plusVal = 0.1f;
    float baseParallax = 0.5f;
    int tileWidth;
    int tileHeight;

    void init(Graphics& graphics);
    void render(SDL_Renderer* renderer, Camera& camera);
};

#endif // BACKGROUND_H_INCLUDED
