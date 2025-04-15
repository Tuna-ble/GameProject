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

void Sprite::render(SDL_Renderer* renderer, Vector2D& position, Camera& camera, int size, float angle) {
    const SDL_Rect* clip = getCurrentClip();
    float offset = size / 2.0f + 10.0f; // khoảng cách từ tâm tàu về phía sau

// Vector hướng tàu
    float rad = angle * M_PI / 180.0f;

    // Tính hướng theo góc xoay
    Vector2D direction(cos(rad), sin(rad));

// Vị trí của lửa (phía sau tàu)
    Vector2D thrusterPos = position - direction * offset;
    SDL_Rect dst {
        static_cast<int>(thrusterPos.x - camera.position.x + 25),
        static_cast<int>(thrusterPos.y - camera.position.y + 25),
        50,
        50
    };

    SDL_Point center = { 25, 25 };
//    Vector2D drawPos = position - camera.position + Vector2D(size / 5, size / 2);
//    SDL_Rect dst = { (int)drawPos.x, (int)drawPos.y, 50, 50 };
    SDL_RenderCopyEx(renderer, texture, clip, &dst, angle, &center, SDL_FLIP_NONE);
}
