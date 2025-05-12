#ifndef SKILL_H_INCLUDED
#define SKILL_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"

struct Shield {
    SDL_Texture* shieldTexture;
    float duration = 3.0f;
    float cooldown = 15.0f;
    float activeTime;
    float cooldownTime;
    bool active;

    void init(SDL_Texture* texture);
    void activate();
    void update(float deltaTime);
    bool isShielding() const;
    bool canActivate() const;
    float getCDPercent() const;
    void reset();
    void render(SDL_Renderer* renderer, const SDL_Rect& playerRect);
};

#endif // SKILL_H_INCLUDED
