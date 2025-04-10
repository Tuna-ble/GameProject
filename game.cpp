#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_image.h>
#include "game.h"
#include "player.h"
#include "background.h"

void Game::init()
{
    graphics.init();
    background = graphics.loadTexture("background1.png");

    spaceShip = graphics.loadTexture("spaceships.png");

    bullet = graphics.loadTexture("projectiles.png");

    player.init(bullet);

    /*cursor = graphics.loadTexture("cursor.png");
    SDL_Rect cursorRect = { x-16, y-16, 32, 32 };
    SDL_RenderCopy(graphics.renderer, cursor, NULL, &cursorRect);*/
}

void Game::update(float deltaTime)
{
    player.handleInput(bullet, camera);
    player.update(deltaTime);
    player.bullets.update(deltaTime);
    camera.update(player);
}

void Game::render()
{
    SDL_RenderClear(graphics.renderer);
    tile.render(graphics.renderer, background, camera);

    //camera.getViewRect();
    player.render(graphics.renderer, spaceShip, camera, ID);
    player.bullets.render(graphics.renderer, camera);

    graphics.presentScene();
}

void Game::run()
{
    Uint32 lastTime = SDL_GetTicks();
    while (player.gameRunning)
    {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        update(deltaTime);
        render();

        int frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < FRAME_DELAY)
            SDL_Delay(FRAME_DELAY - frameTime);
        Uint32 fullFrameTime = SDL_GetTicks() - currentTime;
        float avgFPS = 1000.0f / fullFrameTime;
        //std::cerr << "FPS: " << avgFPS << "\n";
    }
    quit();
}

void Game::quit()
{
    SDL_DestroyTexture(spaceShip);
    spaceShip = NULL;
    SDL_DestroyTexture(background);
    background = NULL;
    IMG_Quit();
    SDL_DestroyRenderer(graphics.renderer);
    SDL_DestroyWindow(graphics.window);
    SDL_Quit();
}
