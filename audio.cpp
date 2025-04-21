#include "audio.h"

void Audio::init() {
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
    }
}

void Audio::loadMusic(const char* path)
{
    music = Mix_LoadMUS(path);
    if (music == nullptr) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                       SDL_LOG_PRIORITY_ERROR,
            "Could not load music! SDL_mixer Error: %s", Mix_GetError());
    }
}

void Audio::playMusic()
{
    if (!musicEnabled) {

        return;
    }
    if (music == nullptr) return;

    if (Mix_PlayingMusic() == 0) {
        Mix_FadeInMusic( music, -1, 3000 );
    }
    else if( Mix_PausedMusic() == 1 ) {
        Mix_ResumeMusic();
    }
}

void Audio::loadSound(const char* name, const char* path) {
    if (SFX.count(name) == 0) {
        Mix_Chunk* chunk = Mix_LoadWAV(path);
        if (chunk == nullptr) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                       SDL_LOG_PRIORITY_ERROR,
           "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        }
        SFX[name] = chunk;
    }
}

void Audio::playSound(const char* name) {
    if (!sfxEnabled) {
        return;
    }
    auto it = SFX.find(name);
    if (it != SFX.end()) {
        Mix_PlayChannel(-1, it->second, 0);
    }
    else {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

void Audio::stopMusic() {
    Mix_HaltMusic();
}

void Audio::cleanUp() {
    Mix_FreeChunk;
    Mix_FreeMusic;
    Mix_CloseAudio();
}
