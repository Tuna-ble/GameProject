#include "gameUI.h"

// ==== UI Button ====

void UIButton::init(Graphics& graphics, const char* textname, TTF_Font* textFont, SDL_Color color, SDL_Rect _rect) {
    font = textFont;
    text = graphics.renderText(textname, font, color);
    rect = _rect;
}

void UIButton::updateHover(int mouseX, int mouseY) {
    SDL_Point p = {mouseX, mouseY};
    mouseHover = SDL_PointInRect( &p, &rect );
}

bool UIButton::isClicked(SDL_Event& e) {
    return mouseHover && e.type == SDL_MOUSEBUTTONDOWN;
}

void UIButton::render(SDL_Renderer* renderer) {
    if (mouseHover) {
        SDL_SetRenderDrawColor(renderer, 70, 170, 255, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 50, 130, 200, 255);
    }
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 30, 90, 150, 255);
    SDL_RenderDrawRect(renderer, &rect);

    SDL_RenderCopy(renderer, text, nullptr, &rect);
}

// ==== Main Menu ====

MainMenu::MainMenu(gameState& s) : state(s) {}

void MainMenu::init(Graphics& graphics, TTF_Font* textFont) {
    font = textFont;

    titleText = graphics.renderText("SPACE WAR", font, textColor);
    playButton.init(graphics, "Play", font, textColor, { 360, 300, 180, 50 });
    settingsButton.init(graphics, "Settings", font, textColor, { 360, 355, 180, 50 });
    quitButton.init(graphics, "Quit", font, textColor, { 360, 410, 180, 50 });

    backgroundTexture = graphics.loadTexture("assets/background1.png");

    titleRect = { 200, 150, 500, 100 };
}

void MainMenu::handleEvent(SDL_Event& e, int mouseX, int mouseY) {
    playButton.updateHover(mouseX, mouseY);
    settingsButton.updateHover(mouseX, mouseY);
    quitButton.updateHover(mouseX, mouseY);

    if (playButton.isClicked(e)) {
        state = gameState::PLAY;
    }
    else if (settingsButton.isClicked(e)) {
        state = gameState::SETTINGS;
    }
    else if (quitButton.isClicked(e)) {
        state = gameState::QUIT;
    }
}

void MainMenu::render(SDL_Renderer* renderer) {
    SDL_RenderCopy( renderer, backgroundTexture, NULL, NULL);

    SDL_RenderCopy(renderer, titleText, nullptr, &titleRect);
    playButton.render(renderer);
    settingsButton.render(renderer);
    quitButton.render(renderer);
}

void MainMenu::cleanUp() {
    SDL_DestroyTexture(titleText);
    SDL_DestroyTexture(playButton.text);
    SDL_DestroyTexture(settingsButton.text);
    SDL_DestroyTexture(quitButton.text);
}

// ==== Pause Menu ====

PauseMenu::PauseMenu(gameState& s) : state(s) {}

void PauseMenu::init(Graphics& graphics, TTF_Font* textFont) {
    font = textFont;

    pausedText = graphics.renderText("PAUSED", font, textColor);
    resumeButton.init(graphics, "Resume", font, textColor, { 360, 250, 180, 50 });
    settingsButton.init(graphics, "Settings", font, textColor, { 360, 305, 180, 50 });
    mainMenuButton.init(graphics, "Main Menu", font, textColor, { 360, 360, 180, 50 });
    quitButton.init(graphics, "Quit", font, textColor, { 360, 415, 180, 50 });

    backgroundTexture = graphics.loadTexture("assets/background1.png");

    pausedRect = { 300, 150, 300, 75 };
}

void PauseMenu::handleEvent(SDL_Event& e, int mouseX, int mouseY) {
    resumeButton.updateHover(mouseX, mouseY);
    settingsButton.updateHover(mouseX, mouseY);
    mainMenuButton.updateHover(mouseX, mouseY);
    quitButton.updateHover(mouseX, mouseY);

    if (resumeButton.isClicked(e)) {
        state = gameState::PLAY;
    }
    else if (settingsButton.isClicked(e)) {
        state = gameState::SETTINGS;
    }
    else if (mainMenuButton.isClicked(e)) {
        state = gameState::MAIN_MENU;
    }
    else if (quitButton.isClicked(e)) {
        state = gameState::QUIT;
    }
}

void PauseMenu::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    SDL_RenderCopy(renderer, pausedText, NULL, &pausedRect);
    resumeButton.render(renderer);
    settingsButton.render(renderer);
    mainMenuButton.render(renderer);
    quitButton.render(renderer);
}

void PauseMenu::cleanUp() {
    SDL_DestroyTexture(pausedText);
    SDL_DestroyTexture(resumeButton.text);
    SDL_DestroyTexture(settingsButton.text);
    SDL_DestroyTexture(mainMenuButton.text);
    SDL_DestroyTexture(quitButton.text);
}
