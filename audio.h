#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include <map>
#include <string>
#include "def.h"

struct Audio {
    std::map<std::string, Mix_Chunk*> SFX;
    bool musicEnabled = true;
    bool sfxEnabled = true;
    Mix_Music* music;

    void init();
    void loadMusic(const char* path);
    void playMusic();
    void loadSound(const char* name, const char* path);
    int playSound(const char* name);
    void stopMusic();
    void cleanUp();
};

#endif // AUDIO_H_INCLUDED
