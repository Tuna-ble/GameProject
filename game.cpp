#include "game.h"

Game::Game() : mainMenu(currentState), pauseMenu(currentState), settingsMenu(currentState), gameOver(currentState), hud(currentState) {}

void Game::init()
{
    graphics.init();
    musicAndSFX.init();

    font = graphics.loadFont("assets/font.ttf", 300);

    musicAndSFX.loadMusic("audio/OST.mp3");
    musicAndSFX.loadSound("shoot", "audio/shoot.flac");
    musicAndSFX.loadSound("hit", "audio/hit.mp3");
    musicAndSFX.loadSound("explosion", "audio/explosion.mp3");
    musicAndSFX.loadSound("hover", "audio/hover.mp3");
    musicAndSFX.loadSound("click", "audio/click.mp3");

    graphics.loadTexture("background", "assets/background1.png");

    graphics.loadTexture("spaceShip", "assets/spaceships.png");

    graphics.loadTexture("bullet", "assets/projectiles.png");

    graphics.loadTexture("enemy", "assets/spaceships.png");

    graphics.loadTexture("asteroid", "assets/asteroid.png");

    graphics.loadTexture("thruster", "assets/thruster.png");

    graphics.loadTexture("healthBar", "assets/healthBar.png");

    graphics.loadTexture("health", "assets/health.png");

    graphics.loadTexture("power-up", "assets/power-up.png");

    HealthBar::setTextures( graphics.getTexture("healthBar"), graphics.getTexture("health") );

    currentState = gameState::MAIN_MENU;
    mainMenu.init(graphics, font);
    pauseMenu.init(graphics, font, musicAndSFX);
    settingsMenu.init(graphics, font, musicAndSFX);
    hud.init(font);

    player.init(graphics, musicAndSFX);

    enemies.init(graphics, musicAndSFX);

    asteroids.init(graphics, musicAndSFX);

    /*cursor = graphics.loadTexture("cursor.png");
    SDL_Rect cursorRect = { x-16, y-16, 32, 32 };
    SDL_RenderCopy(graphics.renderer, cursor, NULL, &cursorRect);*/
}

void Game::update(float deltaTime)
{
    player.update(deltaTime, camera);
    camera.update(player);

    enemies.spawn(camera);
    enemies.update(deltaTime, player);

    asteroids.spawn(camera);
    asteroids.update(deltaTime);

    collision.checkAll(enemies.enemies, asteroids.asteroids, player);

    score = enemies.getScore;

    if (hud.win(deltaTime)) {
        currentState = gameState::GAME_OVER_WIN;
        gameOver.init(graphics, font, score);
        hud.cleanUp();
        return;
    }

    else if (player.health.current <= 0) {
        currentState = gameState::GAME_OVER_LOSE;
        gameOver.init(graphics, font, score);
        hud.cleanUp();
        return;
    }
}

void Game::render()
{
    SDL_RenderClear(graphics.renderer);
    tile.render(graphics.renderer, graphics.getTexture("background"), camera);

    //camera.getViewRect();
    player.render(graphics.renderer, camera, ID);

    enemies.render(graphics.renderer, camera);

    asteroids.render(graphics.renderer, camera);

    hud.render(graphics, graphics.renderer, score);

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
                case gameState::SETTINGS:
                    settingsMenu.handleEvent(event, mouseX, mouseY, musicAndSFX);
                    break;
                case gameState::PAUSED:
                    pauseMenu.handleEvent(event, mouseX, mouseY, musicAndSFX);
                    break;
                case gameState::GAME_OVER_WIN:
                case gameState::GAME_OVER_LOSE:
                    gameOver.handleEvent(event, mouseX, mouseY, musicAndSFX);
                    break;
                case gameState::PLAY:
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                        currentState = gameState::PAUSED;
                        break;
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
                musicAndSFX.playMusic();
                mainMenu.render(graphics.renderer);
                break;

            case gameState::SETTINGS:
                settingsMenu.render(graphics.renderer);
                break;

            case gameState::PLAY:
                musicAndSFX.playMusic();
                update(deltaTime);
                render();
                break;

            case gameState::PAUSED:
                pauseMenu.render(graphics.renderer);
                break;

            case gameState::GAME_OVER_WIN:
            case gameState::GAME_OVER_LOSE:
                restart();
                gameOver.render(graphics.renderer);
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
    asteroids.reset();
}

void Game::quit()
{
    mainMenu.cleanUp();
    pauseMenu.cleanUp();
    settingsMenu.cleanUp();
    musicAndSFX.cleanUp();
    SDL_DestroyTexture( text );
    TTF_CloseFont( font );
    graphics.quit();
}
