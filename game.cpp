#include "game.h"

void Game::init()
{
    graphics.init();

    menu.init(graphics);

    background = graphics.loadTexture("assets/background1.png");

    spaceShip = graphics.loadTexture("assets/spaceships.png");

    bullet = graphics.loadTexture("assets/projectiles.png");

    enemy = graphics.loadTexture("assets/spaceships.png");

    sprite = graphics.loadTexture("assets/thrusters.png");

    player.init(bullet, sprite);

    enemies.init(enemy);

    /*cursor = graphics.loadTexture("cursor.png");
    SDL_Rect cursorRect = { x-16, y-16, 32, 32 };
    SDL_RenderCopy(graphics.renderer, cursor, NULL, &cursorRect);*/
}

void Game::update(float deltaTime)
{
    player.handleInput(bullet, camera, quitGame);
    player.update(deltaTime, camera);
    camera.update(player);

    enemies.spawn({500, 500}, enemies.enemyTexture, bullet, sprite, player);
    enemies.update(deltaTime, player);
    collision.checkAll(enemies.enemies, player);
}

void Game::render()
{
    SDL_RenderClear(graphics.renderer);
    tile.render(graphics.renderer, background, camera);

    //camera.getViewRect();
    player.render(graphics.renderer, spaceShip, camera, ID);

    enemies.render(graphics.renderer, camera);

    graphics.renderTexture(text, 200, 200);

    graphics.presentScene();
}

void Game::run()
{
    while (menu.menuON) {
            SDL_RenderClear(graphics.renderer);
            menu.render(graphics.renderer);
            menu.handleEvent(startGame, quitGame);
            graphics.presentScene();
    }
    Uint32 lastTime = SDL_GetTicks();
    if (startGame) {
        menu.cleanUp();
        while (!quitGame)
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
}

void Game::quit()
{
    SDL_DestroyTexture( text );
    TTF_CloseFont( font );
    SDL_DestroyTexture(spaceShip);
    spaceShip = NULL;
    SDL_DestroyTexture(background);
    background = NULL;
    graphics.quit();
}
