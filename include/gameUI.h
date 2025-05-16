#ifndef GAMEUI_H_INCLUDED
#define GAMEUI_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include "graphics.h"
#include "gameState.h"
#include "audio.h"
#include "health.h"
#include "def.h"
#include "score.h"
#include "gameMode.h"
#include "player.h"

enum class buttonType {
    DEFAULT,
    MUSIC,
    SOUND,
    PAUSE
};

struct UIButton {
    SDL_Rect rect;
    SDL_Rect textRect;
    SDL_Texture* text;
    SDL_Texture* buttonTexture;
    SDL_Texture* musicButtonTexture;
    SDL_Texture* soundButtonTexture;
    SDL_Texture* pauseButtonTexture;
    TTF_Font* font;
    Mix_Chunk* chunk;
    buttonType type = buttonType::DEFAULT;

    bool mouseHover = false;
    bool wasHovering = false;

    bool isToggle = false;
    bool isOn = true;

    Audio* SFX;

    UIButton(Audio& audio);
    void init(Graphics& graphics, const char* textname, TTF_Font* textFont, SDL_Color color, SDL_Rect _rect);
    void setType(buttonType _type);
    void setToggle(bool toggle, bool startOn);
    void render(SDL_Renderer* renderer);
    void updateHover(int mouseX, int mouseY);
    bool isClicked(SDL_Event& e);
    bool sfxOn() const;
};

struct MainMenu {
    gameState& state;
    SDL_Texture* backgroundTexture;
    SDL_Texture* titleText;

    UIButton playButton;
    UIButton settingsButton;
    UIButton quitButton;

    TTF_Font* font;
    SDL_Color textColor = {255, 255, 255};
    SDL_Rect titleRect;

    MainMenu(Audio& audio, gameState& s);
    void init(Graphics& graphics, TTF_Font* textFont);
    void handleEvent(SDL_Event& e, int mouseX, int mouseY);
    void render(SDL_Renderer* renderer);
    void cleanUp();
    void run();
};

struct PauseMenu {
    gameState& state;
    SDL_Texture* backgroundTexture;
    SDL_Texture* windowTexture;
    SDL_Texture* pausedText;

    UIButton resumeButton;
    UIButton musicButton;
    UIButton soundButton;
    UIButton mainMenuButton;
    UIButton quitButton;

    TTF_Font* font;
    SDL_Color textColor = {255, 255, 255};
    SDL_Rect pausedRect;
    SDL_Rect windowRect;
    Audio* audioPtr;

    PauseMenu(Audio& audio, gameState& s);
    void init(Graphics& graphics, TTF_Font* textFont, Audio& audio);
    void handleEvent(SDL_Event& e, int mouseX, int mouseY, Audio& audio);
    void render(SDL_Renderer* renderer);
    void cleanUp();
};

struct SettingsMenu {
    gameState& state;
    SDL_Texture* backgroundTexture;
    SDL_Texture* windowTexture;
    SDL_Texture* settingsText;

    UIButton musicButton;
    UIButton soundButton;
    UIButton backButton;

    TTF_Font* font;
    SDL_Color textColor = {255, 255, 255};
    SDL_Rect settingsRect;
    SDL_Rect windowRect;
    Audio* audioPtr;

    SettingsMenu(Audio& audio, gameState& s);
    void init(Graphics& graphics, TTF_Font* textFont, Audio& audio);
    void handleEvent(SDL_Event& e, int mouseX, int mouseY, Audio& audio);
    void render(SDL_Renderer* renderer);
    void cleanUp();
};

struct ModeMenu {
    gameState& state;
    gameMode& mode;
    SDL_Texture* backgroundTexture;
    SDL_Texture* windowTexture;
    SDL_Texture* modeText;

    UIButton normalButton;
    UIButton hardButton;
    UIButton backButton;

    TTF_Font* font;
    SDL_Color textColor = {255, 255, 255};
    SDL_Rect modeRect;
    SDL_Rect windowRect;
    Audio* audioPtr;

    ModeMenu(Audio& audio, gameState& s, gameMode& m);
    void init(Graphics& graphics, TTF_Font* textFont, Audio& audio);
    void handleEvent(SDL_Event& e, int mouseX, int mouseY, Audio& audio);
    void render(SDL_Renderer* renderer);
    void cleanUp();
};

struct GameOver {
    gameState& state;
    SDL_Texture* backgroundTexture;
    SDL_Texture* resultText;
    SDL_Texture* scoreText;
    SDL_Texture* highScoreText;

    UIButton retryButton;
    UIButton mainMenuButton;
    UIButton quitButton;

    TTF_Font* font;
    SDL_Color textColor = {255, 255, 255};
    SDL_Rect resultRect;
    SDL_Rect scoreRect;
    SDL_Rect highScoreRect;

    Score highestScore;
    int highScore;

    GameOver(Audio& audio, gameState& s);
    void init(Graphics& graphics, TTF_Font* textFont, int score);
    void handleEvent(SDL_Event& e, int mouseX, int mouseY, Audio& audio);
    void render(SDL_Renderer* renderer);
    void cleanUp();
};

struct HUD {
    gameState& state;
    SDL_Texture* scoreText = nullptr;
    SDL_Texture* countDownText = nullptr;
    SDL_Texture* damageText = nullptr;
    SDL_Texture* speedText = nullptr;
    Audio* SFX;
    Player* player;

    SDL_Texture* statsBar;
    SDL_Texture* shieldCD;
    SDL_Texture* beamCD;
    SDL_Texture* CDFill;

    UIButton pauseButton;

    int timer = PLAY_TIME;
    float countdownTimer = 0.0f;
    bool countdownActive = true;

    SDL_Rect scoreRect = {20, 20, 150, 30};
    SDL_Rect countDownRect = {20, 55, 150, 30};
    SDL_Rect damageRect = {275, 45, 150, 30};
    SDL_Rect speedRect = {475, 45, 150, 30};

    SDL_Rect statsRect = {250, 45, 400, 30};
    SDL_Rect shieldCDRect = {20, SCREEN_HEIGHT - 65, 150, 20};
    SDL_Rect beamCDRect = {20, SCREEN_HEIGHT - 40, 150, 20};

    TTF_Font* font;
    SDL_Color textColor = {255, 255, 255, 255};

    HUD(Audio& audio, gameState& s);
    void init(Graphics& graphics, TTF_Font* textFont, Audio& audio, Player& player);
    void handleEvent(SDL_Event& e, int mouseX, int mouseY, Audio& audio);
    void render(Graphics& graphics, SDL_Renderer* renderer, int score);
    bool update(float deltaTime);
    void cleanUp();
};

#endif // GAMEUI_H_INCLUDED
