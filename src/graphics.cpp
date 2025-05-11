#include "graphics.h"

void Graphics::logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

void Graphics::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        logErrorAndExit("SDL_init", SDL_GetError());
        SDL_Quit();
    }
    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr)
        logErrorAndExit("CreateWindow", SDL_GetError());

    if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG))
        logErrorAndExit("SDL_image error: ", SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr)
        logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (TTF_Init() == -1) {
            logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ",
                             TTF_GetError());
    }
}

void Graphics::prepareScene(SDL_Texture * background)
{
    SDL_RenderClear( renderer );
    SDL_RenderCopy( renderer, background, NULL, NULL);
}

void Graphics::presentScene()
{
    SDL_RenderPresent(renderer);
}

void Graphics::loadTexture(const char* filename, const char* filepath) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filepath);

    SDL_Texture *texture = IMG_LoadTexture(renderer, filepath);
    if (texture == NULL)
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

    textures[filename] = texture;
}

SDL_Texture* Graphics::getTexture(const char* filename) {
    return textures[filename];
}

void Graphics::renderTexture(SDL_Texture *texture, int x, int y)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

TTF_Font* Graphics::loadFont(const char* path, int size)
{
    TTF_Font* gFont = TTF_OpenFont( path, size );
    if (gFont == nullptr) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                       SDL_LOG_PRIORITY_ERROR,
                       "Load font %s", TTF_GetError());
    }
    return gFont;
}

SDL_Texture* Graphics::renderText(const char* text,
                            TTF_Font* font, SDL_Color textColor)
{
    SDL_Surface* textSurface =
            TTF_RenderText_Solid( font, text, textColor );
    if( textSurface == nullptr ) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                       SDL_LOG_PRIORITY_ERROR,
                       "Render text surface %s", TTF_GetError());
        return nullptr;
    }

    SDL_Texture* texture =
            SDL_CreateTextureFromSurface( renderer, textSurface );
    if( texture == nullptr ) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                       SDL_LOG_PRIORITY_ERROR,
                       "Create texture from text %s", SDL_GetError());
    }
    SDL_FreeSurface( textSurface );
    return texture;
}

void Graphics::quit() {
    TTF_Quit();
    IMG_Quit();
    for (auto& [id, tex] : textures) {
        SDL_DestroyTexture(tex);
    }
    textures.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
