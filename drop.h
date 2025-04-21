#ifndef DROP_H_INCLUDED
#define DROP_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "vector2D.h"
#include "audio.h"
#include "def.h"
#include "background.h"
#include "dropType.h"

struct Drop {
    Vector2D position;
    SDL_Texture* texture;
    SDL_Rect dest;
    SDL_Rect srcRect;
    dropType type;

    int buffValue;
    bool active;

    float activeTimer = 0.0f;
    float activeDuration = 25.0f;

    Drop(Vector2D position, SDL_Texture* texture, SDL_Rect dest, int ID);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, const Camera& camera);
};

struct DropManager {
    std::vector<Drop> drops;

    SDL_Texture* dropTexture;
    Graphics* asset;

    void init(Graphics& graphics);
    void spawn(Vector2D position);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, const Camera& camera);
};

#endif // DROP_H_INCLUDED
