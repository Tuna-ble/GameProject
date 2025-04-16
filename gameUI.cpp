#include "gameUI.h"

void MainMenu::init(Graphics& graphics) {
    font = graphics.loadFont("assets/font.ttf", 48);

    titleText = graphics.renderText("SPACE WAR", font, textColor);
    playText = graphics.renderText("Play", font, textColor);
    settingsText = graphics.renderText("Settings", font, textColor);
    quitText = graphics.renderText("Quit", font, textColor);

    backgroundTexture = graphics.loadTexture("assets/background1.png");

    titleRect = { 300, 100, 300, 60 };
    playButton = { 360, 200, 180, 50 };
    settingsButton = { 360, 250, 180, 50 };
    quitButton = { 360, 300, 180, 50 };
}

void MainMenu::handleEvent(bool& startGame, bool& quitGame) {
    SDL_Event event;
    int mousex, mousey;
    SDL_GetMouseState(&mousex, &mousey);
    SDL_Point p = { mousex, mousey };

    mouseInPlay = SDL_PointInRect(&p, &playButton);
    mouseInSettings = SDL_PointInRect(&p, &settingsButton);
    mouseInQuit = SDL_PointInRect(&p, &quitButton);

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quitGame = true;
                exit(0);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (mouseInPlay) {
                    startGame = true;
                    menuON = false;
                    break;
                }
                if (mouseInSettings) {
                    //
                }
                if (mouseInQuit) {
                    quitGame = true;
                    exit(0);
                    break;
                }
        }
    }
}

void MainMenu::render(SDL_Renderer* renderer) {
    SDL_RenderClear( renderer );
    SDL_RenderCopy( renderer, backgroundTexture, NULL, NULL);

    SDL_SetRenderDrawColor(renderer, 50, 130, 200, 255);
    SDL_RenderDrawRect(renderer, &playButton);
    SDL_RenderDrawRect(renderer, &settingsButton);
    SDL_RenderDrawRect(renderer, &quitButton);

    SDL_RenderCopy(renderer, titleText, nullptr, &titleRect);
    SDL_RenderCopy(renderer, playText, nullptr, &playButton);
    SDL_RenderCopy(renderer, settingsText, nullptr, &settingsButton);
    SDL_RenderCopy(renderer, quitText, nullptr, &quitButton);
}

void MainMenu::cleanUp() {
    SDL_DestroyTexture(titleText);
    SDL_DestroyTexture(playText);
    SDL_DestroyTexture(quitText);
    TTF_CloseFont(font);
}
