#ifndef SKILL_H_INCLUDED
#define SKILL_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"

struct Shield {
    SDL_Texture* shieldTexture;
    float duration;
    float cooldown;
    float activeTime;
    float cooldownTime;
    bool active;

    //Shield(float duration, float cooldown);

    void init(SDL_Texture* texture, float duration, float cooldown);
    void activate();
    void update(float deltaTime);
    bool isShielding() const;
    bool canActivate() const;
    void reset();
    void render(SDL_Renderer* renderer, const SDL_Rect& playerRect);
};

#endif // SKILL_H_INCLUDED
