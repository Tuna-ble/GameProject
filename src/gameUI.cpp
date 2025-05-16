#include "gameUI.h"

// ==== UI Button ====

UIButton::UIButton(Audio& audio) : SFX(&audio) {}

void UIButton::init(Graphics& graphics, const char* textname, TTF_Font* textFont, SDL_Color color, SDL_Rect _rect) {
    font = textFont;
    text = graphics.renderText(textname, font, color);
    buttonTexture = graphics.getTexture("button");
    musicButtonTexture = graphics.getTexture("musicButton");
    soundButtonTexture = graphics.getTexture("soundButton");
    pauseButtonTexture = graphics.getTexture("pauseHUD");
    rect = _rect;

    int textW, textH;
    SDL_QueryTexture(text, nullptr, nullptr, &textW, &textH);

    textRect = {
        rect.x + (rect.w - textW) / 2,
        rect.y + (rect.h - textH) / 2,
        textW,
        textH
    };
}

void UIButton::setType(buttonType _type) {
    type = _type;
}

void UIButton::setToggle(bool toggle, bool startOn) {
    isToggle = toggle;
    isOn = startOn;
}

void UIButton::updateHover(int mouseX, int mouseY) {
    SDL_Point p = {mouseX, mouseY};
    bool currentlyHovering = SDL_PointInRect( &p, &rect );
    if (currentlyHovering && !wasHovering && sfxOn()) SFX->playSound("hover");

    wasHovering = currentlyHovering;
    mouseHover = currentlyHovering;
}

bool UIButton::isClicked(SDL_Event& e) {
    if (mouseHover && e.type == SDL_MOUSEBUTTONDOWN) {
        if (sfxOn()) SFX->playSound("click");
        if (isToggle) isOn = !isOn;
        return true;
    }
    return false;
}

void UIButton::render(SDL_Renderer* renderer) {
    SDL_Texture* button = buttonTexture;
    if (isToggle) {
        if (type == buttonType::MUSIC) button = musicButtonTexture;
        else if (type == buttonType::SOUND) button = soundButtonTexture;

        if (!isOn)
            SDL_SetTextureColorMod(button, 120, 120, 120);
        else
            SDL_SetTextureColorMod(button, mouseHover ? 200 : 255, mouseHover ? 200 : 255, 255);

        SDL_RenderCopy(renderer, button, nullptr, &rect);
    }
    else {
        if (type == buttonType::PAUSE) button = pauseButtonTexture;
        SDL_SetTextureColorMod(button, mouseHover ? 200 : 255, mouseHover ? 200 : 255, 255);
        SDL_RenderCopy(renderer, button, nullptr, &rect);
    }

    SDL_RenderCopy(renderer, text, nullptr, &textRect);
}

bool UIButton::sfxOn() const {
    return SFX && SFX->sfxEnabled;
}

// ==== Main Menu ====

MainMenu::MainMenu(Audio& audio, gameState& s) : state(s), playButton(audio),
                                                settingsButton(audio), quitButton(audio) {}

void MainMenu::init(Graphics& graphics, TTF_Font* textFont) {
    font = textFont;

    titleText = graphics.renderText("SPACE WAR", font, textColor);
    playButton.init(graphics, "Play", font, textColor, { 360, 300, 180, 50 });
    settingsButton.init(graphics, "Settings", font, textColor, { 360, 355, 180, 50 });
    quitButton.init(graphics, "Quit", font, textColor, { 360, 410, 180, 50 });

    backgroundTexture = graphics.getTexture("background");

    titleRect = { 200, 150, 500, 100 };
}

void MainMenu::handleEvent(SDL_Event& e, int mouseX, int mouseY) {
    playButton.updateHover(mouseX, mouseY);
    settingsButton.updateHover(mouseX, mouseY);
    quitButton.updateHover(mouseX, mouseY);

    if (playButton.isClicked(e)) {
        state = gameState::MODES;
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

PauseMenu::PauseMenu(Audio& audio, gameState& s) : state(s), resumeButton(audio), musicButton(audio),
                                                soundButton(audio), mainMenuButton(audio), quitButton(audio) {}

void PauseMenu::init(Graphics& graphics, TTF_Font* textFont, Audio& audio) {
    font = textFont;
    audioPtr = &audio;

    musicButton.setType(buttonType::MUSIC);
    soundButton.setType(buttonType::SOUND);

    pausedText = graphics.renderText("PAUSED", font, textColor);
    resumeButton.init(graphics, "Resume", font, textColor, { 360, 250, 180, 50 });
    musicButton.init(graphics, "", font, textColor, { 360, 305, 88, 88 });
    soundButton.init(graphics, "", font, textColor, { 452, 305, 88, 88 });
    mainMenuButton.init(graphics, "Main Menu", font, textColor, { 360, 398, 180, 50 });
    quitButton.init(graphics, "Quit", font, textColor, { 360, 453, 180, 50 });

    backgroundTexture = graphics.getTexture("background");
    windowTexture = graphics.getTexture("pauseWindow");

    windowRect = {310, 215, 280, 320};
    pausedRect = { 300, 150, 300, 75 };
}

void PauseMenu::handleEvent(SDL_Event& e, int mouseX, int mouseY, Audio& audio) {
    resumeButton.updateHover(mouseX, mouseY);
    musicButton.updateHover(mouseX, mouseY);
    soundButton.updateHover(mouseX, mouseY);
    mainMenuButton.updateHover(mouseX, mouseY);
    quitButton.updateHover(mouseX, mouseY);

    if (resumeButton.isClicked(e)) {
        state = gameState::PLAY;
    }
    else if (musicButton.isClicked(e)) {
        audio.musicEnabled = !audio.musicEnabled;
        if (!audio.musicEnabled)
            audio.stopMusic();
        else
            audio.playMusic();

        musicButton.setToggle(true, audio.musicEnabled);
    }
    else if (soundButton.isClicked(e)) {
        audio.sfxEnabled = !audio.sfxEnabled;

        soundButton.setToggle(true, audio.sfxEnabled);
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
    SDL_RenderCopy(renderer, windowTexture, NULL, &windowRect);

    musicButton.setToggle(true, audioPtr->musicEnabled);
    soundButton.setToggle(true, audioPtr->sfxEnabled);

    SDL_RenderCopy(renderer, pausedText, NULL, &pausedRect);
    resumeButton.render(renderer);
    musicButton.render(renderer);
    soundButton.render(renderer);
    mainMenuButton.render(renderer);
    quitButton.render(renderer);
}

void PauseMenu::cleanUp() {
    SDL_DestroyTexture(pausedText);
    SDL_DestroyTexture(resumeButton.text);
    SDL_DestroyTexture(musicButton.text);
    SDL_DestroyTexture(soundButton.text);
    SDL_DestroyTexture(mainMenuButton.text);
    SDL_DestroyTexture(quitButton.text);
}

// ==== Settings Menu =====

SettingsMenu::SettingsMenu(Audio& audio, gameState& s) : state(s), musicButton(audio),
                                                        soundButton(audio), backButton(audio) {}

void SettingsMenu::init(Graphics& graphics, TTF_Font* textFont, Audio& audio) {
    font = textFont;
    audioPtr = &audio;

    musicButton.setType(buttonType::MUSIC);
    soundButton.setType(buttonType::SOUND);

    settingsText = graphics.renderText("Settings", font, textColor);
    musicButton.init(graphics, "", font, textColor, { 338, 265, 110, 110 });
    soundButton.init(graphics, "", font, textColor, { 452, 265, 110, 110 });
    backButton.init(graphics, "Back", font, textColor, { 338, 380, 224, 62 });

    backgroundTexture = graphics.getTexture("background");
    windowTexture = graphics.getTexture("settingWindow");

    windowRect = { 285, 220, 330, 260 };
    settingsRect = { 300, 150, 300, 75 };
}
void SettingsMenu::handleEvent(SDL_Event& e, int mouseX, int mouseY, Audio& audio) {
    musicButton.updateHover(mouseX, mouseY);
    soundButton.updateHover(mouseX, mouseY);
    backButton.updateHover(mouseX, mouseY);

    if (musicButton.isClicked(e)) {
        audio.musicEnabled = !audio.musicEnabled;
        if (!audio.musicEnabled)
            audio.stopMusic();
        else
            audio.playMusic();

        musicButton.setToggle(true, audio.musicEnabled);
    }
    else if (soundButton.isClicked(e)) {
        audio.sfxEnabled = !audio.sfxEnabled;

        soundButton.setToggle(true, audio.sfxEnabled);
    }
    else if (backButton.isClicked(e)) {
        state = gameState::MAIN_MENU;
    }
}
void SettingsMenu::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer, windowTexture, NULL, &windowRect);

    musicButton.setToggle(true, audioPtr->musicEnabled);
    soundButton.setToggle(true, audioPtr->sfxEnabled);

    SDL_RenderCopy(renderer, settingsText, NULL, &settingsRect);
    musicButton.render(renderer);
    soundButton.render(renderer);
    backButton.render(renderer);
}
void SettingsMenu::cleanUp() {
    SDL_DestroyTexture(settingsText);
    SDL_DestroyTexture(musicButton.text);
    SDL_DestroyTexture(soundButton.text);
    SDL_DestroyTexture(backButton.text);
}

// ==== Mode Menu =====

ModeMenu::ModeMenu(Audio& audio, gameState& s, gameMode& m) : state(s), mode(m), normalButton(audio),
                                                            hardButton(audio), backButton(audio) {}

void ModeMenu::init(Graphics& graphics, TTF_Font* textFont, Audio& audio) {
    font = textFont;
    audioPtr = &audio;

    modeText = graphics.renderText("Modes", font, textColor);
    normalButton.init(graphics, "Normal", font, textColor, { 340, 250, 220, 60 });
    hardButton.init(graphics, "Hard", font, textColor, { 340, 315, 220, 60 });
    backButton.init(graphics, "Back", font, textColor, { 340, 380, 220, 60 });

    backgroundTexture = graphics.getTexture("background");
    //windowTexture = graphics.getTexture("settingWindow");

    //windowRect = { 285, 220, 330, 260 };
    modeRect = { 300, 150, 300, 75 };
}
void ModeMenu::handleEvent(SDL_Event& e, int mouseX, int mouseY, Audio& audio) {
    normalButton.updateHover(mouseX, mouseY);
    hardButton.updateHover(mouseX, mouseY);
    backButton.updateHover(mouseX, mouseY);

    if (normalButton.isClicked(e)) {
        state = gameState::PLAY;
        mode = gameMode::NORMAL;
    }
    else if (hardButton.isClicked(e)) {
        state = gameState::PLAY;
        mode = gameMode::HARD;
    }
    else if (backButton.isClicked(e)) {
        state = gameState::MAIN_MENU;
    }
}
void ModeMenu::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    //SDL_RenderCopy(renderer, windowTexture, NULL, &windowRect);

    SDL_RenderCopy(renderer, modeText, NULL, &modeRect);
    normalButton.render(renderer);
    hardButton.render(renderer);
    backButton.render(renderer);
}
void ModeMenu::cleanUp() {
    SDL_DestroyTexture(modeText);
    SDL_DestroyTexture(normalButton.text);
    SDL_DestroyTexture(hardButton.text);
    SDL_DestroyTexture(backButton.text);
}

// ==== Game Over ====

GameOver::GameOver(Audio& audio, gameState& s) : state(s), retryButton(audio),
                                                mainMenuButton(audio), quitButton(audio) {}

void GameOver::init(Graphics& graphics, TTF_Font* textFont, int score) {
    font = textFont;
    highScore = highestScore.loadHighScore();

    const char* result;
    std::string currentScore = "Score: " + std::to_string(score);
    std::string bestScore = "Best: " + std::to_string(highScore);

    if (state == gameState::GAME_OVER_WIN)
        result = "YOU WIN";
    else if (state == gameState::GAME_OVER_LOSE)
        result = "YOU LOSE";

    resultText = graphics.renderText(result, font, textColor);
    scoreText = graphics.renderText(currentScore.c_str(), font, textColor);
    highScoreText = graphics.renderText(bestScore.c_str(), font, textColor);
    retryButton.init(graphics, "Retry", font, textColor, { 360, 320, 180, 50 });
    mainMenuButton.init(graphics, "Main Menu", font, textColor, { 360, 375, 180, 50 });
    quitButton.init(graphics, "Quit", font, textColor, { 360, 430, 180, 50 });

    backgroundTexture = graphics.getTexture("background");

    resultRect = { 300, 150, 300, 75 };
    scoreRect = { 200, 250, 200, 50 };
    highScoreRect = { 500, 250, 200, 50 };
}

void GameOver::handleEvent(SDL_Event& e, int mouseX, int mouseY, Audio& audio) {
    retryButton.updateHover(mouseX, mouseY);
    mainMenuButton.updateHover(mouseX, mouseY);
    quitButton.updateHover(mouseX, mouseY);

    if (retryButton.isClicked(e)) {
        state = gameState::PLAY;
    }
    else if (mainMenuButton.isClicked(e)) {
        state = gameState::MAIN_MENU;
    }
    else if (quitButton.isClicked(e)) {
        state = gameState::QUIT;
    }
}

void GameOver::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    SDL_RenderCopy(renderer, resultText, NULL, &resultRect);
    SDL_RenderCopy(renderer, scoreText, NULL, &scoreRect);
    SDL_RenderCopy(renderer, highScoreText, NULL, &highScoreRect);
    retryButton.render(renderer);
    mainMenuButton.render(renderer);
    quitButton.render(renderer);
}

void GameOver::cleanUp() {
    SDL_DestroyTexture(resultText);
    SDL_DestroyTexture(scoreText);
    SDL_DestroyTexture(highScoreText);
    SDL_DestroyTexture(resultText);
    SDL_DestroyTexture(retryButton.text);
    SDL_DestroyTexture(mainMenuButton.text);
    SDL_DestroyTexture(quitButton.text);
}

// ==== HUD =====

HUD::HUD(Audio& audio, gameState& s) : state(s), pauseButton(audio) {}

void HUD::init(Graphics& graphics, TTF_Font* textFont, Audio& audio, Player& _player) {
    font = textFont;
    SFX = &audio;
    player = &_player;
    countdownActive = true;

    pauseButton.setType(buttonType::PAUSE);
    pauseButton.init(graphics, "", font, textColor, { SCREEN_WIDTH - 60, 20, 40, 40 });

    statsBar = graphics.getTexture("statsBar");
    beamCD = graphics.getTexture("beamBar");
    shieldCD = graphics.getTexture("shieldBar");
    CDFill = graphics.getTexture("fill");
}

void HUD::handleEvent(SDL_Event& e, int mouseX, int mouseY, Audio& audio) {
    pauseButton.updateHover(mouseX, mouseY);

    if (pauseButton.isClicked(e)) {
        state = gameState::PAUSED;
    }
}

bool HUD::update(float deltaTime) {
    if (countdownActive) {
    countdownTimer += deltaTime;

    if (countdownTimer >= 1.0f) {
        countdownTimer -= 1.0f;

        if (timer == 6) {
            SFX->playSound("time");
        }

        timer--;

        if (timer <= 0) {
            countdownActive = false;
            timer = 0;
            return true;
        }
    }
    }
    return false;
}

void HUD::render(Graphics& graphics, SDL_Renderer* renderer, int score) {
    SDL_Rect beamFillRect = {20, SCREEN_HEIGHT - 40, 150 * player->getBeamCDPercent(), 20};
    SDL_Rect shieldFillRect = {20, SCREEN_HEIGHT - 65, 150 * player->shield.getCDPercent(), 20};

    SDL_RenderCopy(renderer, statsBar, nullptr, &statsRect);
    SDL_RenderCopy(renderer, beamCD, nullptr, &beamCDRect);
    SDL_RenderCopy(renderer, CDFill, nullptr, &beamFillRect);

    SDL_RenderCopy(renderer, shieldCD, nullptr, &shieldCDRect);
    SDL_RenderCopy(renderer, CDFill, nullptr, &shieldFillRect);

    std::string scoreStr = "Score: " + std::to_string(score);
    std::string countStr = "Time: " + std::to_string(timer);

    std::string damage = "Damage: " + std::to_string(player->damage);
    std::string speed = "Speed: " + std::to_string(player->speed);

    if (scoreText) SDL_DestroyTexture(scoreText);
    if (countDownText) SDL_DestroyTexture(countDownText);
    if (damageText) SDL_DestroyTexture(damageText);
    if (speedText) SDL_DestroyTexture(speedText);

    scoreText = graphics.renderText(scoreStr.c_str(), font, textColor);
    countDownText = graphics.renderText(countStr.c_str(), font, textColor);
    damageText = graphics.renderText(damage.c_str(), font, textColor);
    speedText = graphics.renderText(speed.c_str(), font, textColor);

    SDL_RenderCopy(renderer, scoreText, nullptr, &scoreRect);
    SDL_RenderCopy(renderer, countDownText, nullptr, &countDownRect);
    SDL_RenderCopy(renderer, damageText, nullptr, &damageRect);
    SDL_RenderCopy(renderer, speedText, nullptr, &speedRect);

    pauseButton.render(renderer);
}

void HUD::cleanUp() {
    SDL_DestroyTexture(scoreText);
    SDL_DestroyTexture(countDownText);
    scoreText = nullptr;
    countDownText = nullptr;
    timer = PLAY_TIME;
    countdownActive = true;
}
