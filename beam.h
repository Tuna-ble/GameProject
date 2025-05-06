#ifndef BEAM_H_INCLUDED
#define BEAM_H_INCLUDED
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include "def.h"
#include "vector2D.h"
#include "sprite.h"
#include "audio.h"
#include "bulletFrom.h"

struct Camera;

struct Beam {
    Vector2D position;
    Sprite beamShooting;
    int width = SHIP_SIZE;
    int height = SCREEN_WIDTH * 2;
    bool active;
    float angle;
    int damage;
    bulletFrom shooter;

    SDL_Texture* beamTexture;
    SDL_Rect beamRect;
    Audio* SFX;
    int channel = -1;

    float activeTimer = 0.0f;
    float activeDuration = 3.0f;

    float warningDuration = 1.0f;
    float warningTimer = 0.0f;
    bool fired = false;

    Beam(Vector2D position, int damage, SDL_Texture* tex, float angle, Audio* sound, bool fired, bulletFrom shooter);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, Camera& camera);
    void drawOBB(SDL_Renderer* renderer, const Camera& camera) const;
    bool isFinished() const;
};

struct BeamManager {
    std::vector<Beam> beams;
    SDL_Texture* beamTexture;
    Audio* SFX;

    void init(SDL_Texture* texture, Audio& sound);

    void shoot(Vector2D position, int damage, float angle, bool fired, bulletFrom shooter);

    void update(float deltaTime, Vector2D position, float angle);
    void update(float deltaTime);

    void render(SDL_Renderer* renderer, Camera& camera);

    Vector2D getBeamSpawnPosition(Vector2D& position) const;

    void stopAllBeamSounds();
};

#endif // BEAM_H_INCLUDED
