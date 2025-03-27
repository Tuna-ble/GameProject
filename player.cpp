#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_image.h>
#include "player.h"

void Player::handleInput()
{
    SDL_Event event;
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    while(SDL_PollEvent(&event))
    {
    switch (event.type)
    {
        case SDL_QUIT:
            gameRunning = false;
            exit(0);
            break;
        case SDL_MOUSEBUTTONDOWN:
            std::cerr << "Down at (" << mx << ", " << my << ")\n";
            break;
        case SDL_MOUSEBUTTONUP:
            std::cerr << "Up at (" << mx << ", " << my << ")\n";
            break;
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

void Player::posUpd()
{
    x+=vx;
    y+=vy;
}
