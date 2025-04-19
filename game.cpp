#include "game.h"

Game::Game() : mainMenu(currentState), pauseMenu(currentState) {}

void Game::init()
{
    graphics.init();

    font = graphics.loadFont("assets/font.ttf", 300);

    currentState = gameState::MAIN_MENU;
    mainMenu.init(graphics, font);
    pauseMenu.init(graphics, font);

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

    graphics.presentScene();
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();

    while (isRunning) {
        SDL_Event event;
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }

            switch (currentState) {
                case gameState::MAIN_MENU:
                    mainMenu.handleEvent(event, mouseX, mouseY);
                    break;
                case gameState::PAUSED:
                    pauseMenu.handleEvent(event, mouseX, mouseY);
                    break;
                case gameState::PLAY:
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                        currentState = gameState::PAUSED;
                    }
                    player.handleInput(event, camera);
                    break;
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        SDL_RenderClear(graphics.renderer);

        switch (currentState) {
            case gameState::MAIN_MENU:
                restart();
                mainMenu.render(graphics.renderer);
                break;

            case gameState::PLAY:
                update(deltaTime);
                render();
                break;

            case gameState::PAUSED:
//                render();
                pauseMenu.render(graphics.renderer);
                break;

            case gameState::QUIT:
                isRunning = false;
                break;
        }

        graphics.presentScene();

        // optional: cap FPS
        int frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < FRAME_DELAY)
            SDL_Delay(FRAME_DELAY - frameTime);
    }

    quit();
}

void Game::restart() {
    player.reset();
    camera.position = {0, 0};
    enemies.reset();
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
