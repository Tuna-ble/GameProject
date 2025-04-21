#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED
#include <SDL.h>
#include <SDL_image.h>
#include "def.h"
#include "vector2D.h"

struct Camera;

struct Sprite {
    SDL_Texture* texture;
    std::vector<SDL_Rect> clips;
    int currentFrame = 0;
    int totalFrames = 1;
    float frameTime = 0.1f; // thời gian mỗi frame (giây)
    float elapsedTime = 0.0f;
    bool loop = true;
    bool finished = false;

    void init(SDL_Texture* _texture, int frames, const int _clips [][4]);
    void update();
    const SDL_Rect* getCurrentClip() const;

    void render(SDL_Renderer* renderer, Vector2D& position, Camera& camera, int size, float angle);
    void animate(float deltaTime);
    bool isFinished() const;
};

#endif // SPRITE_H_INCLUDED
