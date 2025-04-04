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

    player.render(graphics.renderer, spaceShip, ID);

    graphics.presentScene();
}

void Game::run()
{
    while (player.gameRunning)
    {
        frameStart = SDL_GetTicks();

        update();
        render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY)
            SDL_Delay(FRAME_DELAY - frameTime);
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
