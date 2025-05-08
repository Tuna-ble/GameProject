#include "sprite.h"
#include "background.h"

void Sprite::init(SDL_Texture* _texture, int frames, const int _clips [][4]) {
    texture = _texture;
    SDL_Rect clip;
    for (int i = 0; i < frames; i++) {
        clip.x = _clips[i][0];
        clip.y = _clips[i][1];
        clip.w = _clips[i][2];
        clip.h = _clips[i][3];
        clips.push_back(clip);
    }
}

void Sprite::update() {
    currentFrame = (currentFrame + 1) % clips.size();
}

const SDL_Rect* Sprite::getCurrentClip() const {
    return &(clips[currentFrame]);
}

void Sprite::render(SDL_Renderer* renderer, Vector2D& position, Camera& camera, SDL_Rect& dest, float angle, SDL_Point* center) {
    const SDL_Rect* clip = getCurrentClip();

    SDL_RenderCopyEx(renderer, texture, clip, &dest, angle, center, SDL_FLIP_NONE);
}

void Sprite::animate(float deltaTime) {
    if (finished) return;

    elapsedTime += deltaTime;
    if (elapsedTime >= frameTime) {
        elapsedTime = 0.0f;
        currentFrame++;

        if (currentFrame >= clips.size()) {
            if (loop) {
                currentFrame = 0;
            }
            else {
                currentFrame = clips.size() - 1;
                finished = true;
            }
        }
    }
}

bool Sprite::isFinished() const {
    return finished;
}
