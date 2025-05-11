#include "game.h"

Game::Game() : mainMenu(currentState), pauseMenu(currentState), settingsMenu(currentState),
            modeMenu(currentState, mode), gameOver(currentState), hud(currentState), enemies(mode) {}

void Game::init()
{
    graphics.init();
    musicAndSFX.init();

    font = graphics.loadFont("assets/font/font.ttf", 25);

    musicAndSFX.loadMusic("assets/audio/OST.mp3");
    musicAndSFX.loadSound("shoot", "assets/audio/shoot.flac");
    musicAndSFX.loadSound("hit", "assets/audio/hit.mp3");
    musicAndSFX.loadSound("explosion", "assets/audio/explosion.mp3");
    musicAndSFX.loadSound("hover", "assets/audio/hover.mp3");
    musicAndSFX.loadSound("click", "assets/audio/click.mp3");
    musicAndSFX.loadSound("getBuff", "assets/audio/getBuff.mp3");
    musicAndSFX.loadSound("time", "assets/audio/timeRunning.mp3");
    musicAndSFX.loadSound("victory", "assets/audio/victory.mp3");
    musicAndSFX.loadSound("failure", "assets/audio/failed.mp3");
    musicAndSFX.loadSound("charge-up", "assets/audio/charge-up.mp3");
    musicAndSFX.loadSound("beam-shoot", "assets/audio/beam-shoot.mp3");

    graphics.loadTexture("background", "assets/images/background0.png");
    graphics.loadTexture("background1", "assets/images/background1.png");
    graphics.loadTexture("background2", "assets/images/background2.png");
    graphics.loadTexture("background3", "assets/images/background3.png");
    graphics.loadTexture("background4", "assets/images/background4.png");
    graphics.loadTexture("background5", "assets/images/background5.png");
    graphics.loadTexture("background6", "assets/images/background6.png");

    graphics.loadTexture("spaceShip", "assets/images/spaceships.png");

    graphics.loadTexture("bullet", "assets/images/projectiles.png");

    graphics.loadTexture("enemy", "assets/images/spaceships.png");

    graphics.loadTexture("asteroid", "assets/images/asteroid.png");

    graphics.loadTexture("thruster", "assets/images/thruster.png");

    graphics.loadTexture("healthBar", "assets/images/healthBar.png");

    graphics.loadTexture("health", "assets/images/health.png");

    graphics.loadTexture("power-up", "assets/images/power-up.png");

    graphics.loadTexture("explosion", "assets/images/explosion.png");

    graphics.loadTexture("asteroid_explode", "assets/images/asteroid_explode.png");

    graphics.loadTexture("button", "assets/images/button.png");

    graphics.loadTexture("musicButton", "assets/images/music.png");

    graphics.loadTexture("soundButton", "assets/images/sound.png");

    graphics.loadTexture("pauseWindow", "assets/images/wdpopup.png");

    graphics.loadTexture("settingWindow", "assets/images/table.png");

    graphics.loadTexture("pauseHUD", "assets/images/pause.png");

    graphics.loadTexture("shield", "assets/images/shield.png");

    graphics.loadTexture("beam-active", "assets/images/beam-active.png");
    graphics.loadTexture("beam-active0", "assets/images/beam-active0.png");

    graphics.loadTexture("speed", "assets/images/speed.png");

    graphics.loadTexture("damage", "assets/images/damage.png");

    HealthBar::setTextures( graphics.getTexture("healthBar"), graphics.getTexture("health") );

    currentState = gameState::MAIN_MENU;
    mainMenu.init(graphics, font);
    pauseMenu.init(graphics, font, musicAndSFX);
    settingsMenu.init(graphics, font, musicAndSFX);
    modeMenu.init(graphics, font, musicAndSFX);
    hud.init(graphics, font, musicAndSFX);

    tile.init(graphics);

    player.init(graphics, musicAndSFX);

    enemies.init(graphics, musicAndSFX);

    asteroids.init(graphics, musicAndSFX);

    drops.init(graphics);

    explosionManager.init(graphics);
}

void Game::update(float deltaTime)
{
    player.update(deltaTime, camera);
    camera.update(player);

    enemies.spawn(camera);
    enemies.update(deltaTime, graphics, player, drops, explosionManager);

    asteroids.spawn(camera);
    asteroids.update(deltaTime, explosionManager);

    drops.update(deltaTime);

    explosionManager.update(deltaTime);

    collision.checkAll(enemies.enemies, asteroids.asteroids, drops.drops, player);

    score = enemies.getScore;

    if (hud.update(deltaTime)) {
        currentState = gameState::GAME_OVER_WIN;
        if (score > highScore.loadHighScore()) {
        highScore.saveHighScore(score);
        }
        gameOver.init(graphics, font, score);
        for (auto& e : enemies.enemies)
            e.beams.stopAllBeamSounds();
        player.beams.stopAllBeamSounds();
        musicAndSFX.playSound("victory");
        hud.cleanUp();
        return;
    }

    else if (player.health.isDead()) {
        currentState = gameState::GAME_OVER_LOSE;
        gameOver.init(graphics, font, score);
        for (auto& e : enemies.enemies)
            e.beams.stopAllBeamSounds();
        player.beams.stopAllBeamSounds();
        musicAndSFX.playSound("failure");
        hud.cleanUp();
        return;
    }
}

void Game::render()
{
    SDL_RenderClear(graphics.renderer);

    tile.render(graphics.renderer, camera);

    player.render(graphics.renderer, camera);

    enemies.render(graphics.renderer, camera);

    asteroids.render(graphics.renderer, camera);

    drops.render(graphics.renderer, camera);

    explosionManager.render(graphics.renderer, camera);

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
                case gameState::MODES:
                    modeMenu.handleEvent(event, mouseX, mouseY, musicAndSFX);
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
                    hud.handleEvent(event, mouseX, mouseY, musicAndSFX);
                    player.handleInput(event, camera);
                    break;
                case gameState::QUIT:
                    isRunning = false;
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
                hud.cleanUp();
                musicAndSFX.playMusic();
                mainMenu.render(graphics.renderer);
                break;

            case gameState::SETTINGS:
                settingsMenu.render(graphics.renderer);
                break;

            case gameState::MODES:
                modeMenu.render(graphics.renderer);
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
                musicAndSFX.stopMusic();
                gameOver.render(graphics.renderer);
                break;

            case gameState::QUIT:
                isRunning = false;
                break;
        }

        graphics.presentScene();

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
    drops.reset();
    asteroids.reset();
    explosionManager.explosions.clear();
}

void Game::quit()
{
    mainMenu.cleanUp();
    pauseMenu.cleanUp();
    settingsMenu.cleanUp();
    musicAndSFX.cleanUp();
    TTF_CloseFont( font );
    graphics.quit();
}
