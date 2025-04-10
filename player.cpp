#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "player.h"
#include "background.h"
#include "bullet.h"

void Player::init(SDL_Texture* bulletTexture) {
    bullets.init(bulletTexture);
}

void Player::handleInput(SDL_Texture* texture, Camera &camera)
{
    SDL_Event event;
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    while(SDL_PollEvent(&event))
    {
    switch (event.type)
    {
        case SDL_QUIT:
            gameRunning = false;
            exit(0);
            break;
        case SDL_MOUSEBUTTONDOWN:
            float worldMouseX = mouseX + camera.x;
            float worldMouseY = mouseY + camera.y;

            float dirX = worldMouseX - (x + SHIP_SIZE / 2);
            float dirY = worldMouseY - (y + SHIP_SIZE / 2) ;

            float spawnX = x + SHIP_SIZE / 2 - BULLET_SIZE / 2;
            float spawnY = y + SHIP_SIZE / 2 - BULLET_SIZE / 2;

            float len = std::sqrt(dirX * dirX + dirY * dirY);
            if (len != 0) {
            dirX /= len;
            dirY /= len;
            float angle = atan2(worldMouseY - spawnY - BULLET_SIZE / 2, worldMouseX - spawnX - BULLET_SIZE / 2) * 180 / M_PI + 90;
            bullets.shoot(spawnX, spawnY, dirX, dirY, bullets.bulletSpeed, bulletSrcRect, angle);
            }
            break;
//        case SDL_MOUSEBUTTONUP:
//            std::cerr << "Up at (" << mx << ", " << my << ")\n";
//            break;
    }
    }
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    vx=0;
    vy=0;

    if (currentKeyStates[SDL_SCANCODE_W] ) vy = -speed;
    if (currentKeyStates[SDL_SCANCODE_S] ) vy = speed;
    if (currentKeyStates[SDL_SCANCODE_A] ) vx = -speed;
    if (currentKeyStates[SDL_SCANCODE_D] ) vx = speed;

}

void Player::update(float deltaTime)
{
    x += vx * deltaTime;
    y += vy * deltaTime;

    x = std::max(0.f, std::min(x, mapWidth - (float)SHIP_SIZE));
    y = std::max(0.f, std::min(y, mapHeight - (float)SHIP_SIZE));

    bullets.update(deltaTime);
}

void Player::render(SDL_Renderer* renderer, SDL_Texture* texture, Camera &camera, int ID)
{
    int drawX = static_cast<int>(x - camera.x);
    int drawY = static_cast<int>(y - camera.y);
    SDL_Rect dest = { drawX, drawY, SHIP_SIZE, SHIP_SIZE };

    int mx, my;
    SDL_GetMouseState(&mx, &my);

    const double angle = atan2(my - drawY - SHIP_SIZE / 2, mx - drawX - SHIP_SIZE / 2) * 180 / M_PI + 90;

    //SDL_RenderDrawLine(graphics.renderer, player.x +24, player.y+24, mx, my);

    SDL_RenderCopyEx(renderer, texture, &srcRect, &dest, angle, NULL, SDL_FLIP_NONE);
}
