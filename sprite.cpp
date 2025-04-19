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

void Sprite::render(SDL_Renderer* renderer, Vector2D& position, Camera& camera, int size) {
    const SDL_Rect* clip = getCurrentClip();

    Vector2D drawPos = position - camera.position;
    SDL_Rect dest = { (int)drawPos.x, (int)drawPos.y, size, size };
    SDL_RenderCopy(renderer, texture, clip, &dest);
}
