#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_image.h>
#include "game.h"
#include "player.h"

#define ID 0

void Game::init()
{
    graphics.init();
    background = graphics.loadTexture("background1.png");

    spaceShip = graphics.loadTexture("spaceships.png");

    /*cursor = graphics.loadTexture("cursor.png");
    SDL_Rect cursorRect = { x-16, y-16, 32, 32 };
    SDL_RenderCopy(graphics.renderer, cursor, NULL, &cursorRect);*/
}

void Game::update()
{
    player.handleInput();
    player.posUpd();
}

void Game::render()
{
    SDL_RenderClear(graphics.renderer);
    graphics.prepareScene(background);

    SDL_Rect dest = { player.x, player.y, 48, 48 };
    SDL_Rect srcRect = { (ID % 2) * 48, (ID / 2) * 48, 48, 48 };

    int mx, my;
    SDL_GetMouseState(&mx, &my);

    const double angle = atan2(my - player.y - 24, mx - player.x - 24) * 180 / M_PI + 90;

    //SDL_RenderDrawLine(graphics.renderer, player.x +24, player.y+24, mx, my);

    SDL_RenderCopyEx(graphics.renderer, spaceShip, &srcRect, &dest, angle, NULL, SDL_FLIP_NONE);

    graphics.presentScene();
}

void Game::run()
{
    while (player.gameRunning)
    {
        update();
        render();
        SDL_Delay(16);
    }
    quit();
}

void Game::quit()
{
    SDL_DestroyTexture( spaceShip );
    spaceShip = NULL;
    SDL_DestroyTexture( background );
    background = NULL;
    IMG_Quit();
    SDL_DestroyRenderer(graphics.renderer);
    SDL_DestroyWindow(graphics.window);
    SDL_Quit();
}
