#include "beam.h"
#include "background.h"

// ==== Beam ====

Beam::Beam(Vector2D position, int damage, SDL_Texture* tex, float angle, Audio* sound, bool fired, bulletFrom shooter)
    : position(position), damage(damage), beamTexture(tex),
    angle(angle), shooter(shooter), active(true), fired(fired), SFX(sound) {
        beamShooting.init(beamTexture, BEAM_FRAMES, BEAM_CLIPS);

        beamShooting.loop = true;
        beamShooting.currentFrame = 0;
        beamShooting.elapsedTime = 0.0f;
        beamShooting.finished = false;
    }

void Beam::update(float deltaTime) {
    if (!fired) {
        warningTimer += deltaTime;
        if (warningTimer >= warningDuration) {
            fired = true;
        }
    }
    else {
        beamShooting.update();
        if (fired && channel == -1) {
            channel = SFX->playSound("beam-shoot");
        }
        activeTimer += deltaTime;
        if (activeTimer >= activeDuration) {
            active = false;
            beamShooting.loop = false;
        }
    }
}

void Beam::render(SDL_Renderer* renderer, Camera& camera) {
    if (!active) return;

    if (!fired) {
        drawOBB(renderer, camera);
    }
    else {

    SDL_Point center = { width / 2, height + width / 2 };

    SDL_Rect dst = {
        static_cast<int>(position.x - camera.position.x ),
        static_cast<int>(position.y - camera.position.y - height ),
        width, height
    };

    beamShooting.render(renderer, position, camera, dst, angle, &center);
    }
}

void Beam::drawOBB(SDL_Renderer* renderer, const Camera& camera) const {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);

    Vector2D corners[4];
    float rad = angle * M_PI / 180.0f;
    float cosA = cos(rad);
    float sinA = sin(rad);

    Vector2D center = { position.x + width / 2, position.y + width / 2};

    Vector2D localCorners[4] = {
        { -width / 2.0f, 0.0f },
        {  width / 2.0f, 0.0f },
        {  width / 2.0f, -height },
        { -width / 2.0f, -height }
    };

    for (int i = 0; i < 4; i++) {
        float rotatedX = localCorners[i].x * cosA - localCorners[i].y * sinA;
        float rotatedY = localCorners[i].x * sinA + localCorners[i].y * cosA;
        corners[i] = { center.x + rotatedX, center.y + rotatedY };
    }

    SDL_Vertex verts[6];

    for (int i = 0; i < 4; i++) {
        corners[i].x -= camera.position.x;
        corners[i].y -= camera.position.y;
    }
    verts[0].position = { corners[0].x, corners[0].y };
    verts[1].position = { corners[1].x, corners[1].y };
    verts[2].position = { corners[2].x, corners[2].y };
    verts[3].position = { corners[0].x, corners[0].y };
    verts[4].position = { corners[2].x, corners[2].y };
    verts[5].position = { corners[3].x, corners[3].y };

    for (int i = 0; i < 6; i++)
        verts[i].color = {255, 0, 0, 100};

    SDL_RenderGeometry(renderer, nullptr, verts, 6, nullptr, 0);
}

bool Beam::isFinished() const {
    return beamShooting.isFinished();
}

// ==== Beam Manager ====

void BeamManager::init(SDL_Texture* texture, Audio& sound) {
    beamTexture = texture;
    SFX = &sound;
}

void BeamManager::shoot(Vector2D position, int damage, float angle, bool fired, bulletFrom shooter) {
    beams.emplace_back(position, damage, beamTexture, angle, SFX, fired, shooter);
}

void BeamManager::update(float deltaTime, Vector2D position, float angle) {
    for (auto& b : beams) {
        b.update(deltaTime);
        b.position = position;
        b.angle = angle;
    }
    beams.erase(std::remove_if(beams.begin(), beams.end(),
                                 [](const Beam& b) { return !b.active; }), beams.end());
}

void BeamManager::update(float deltaTime) {
    for (auto& b : beams) {
        b.update(deltaTime);
    }
    beams.erase(std::remove_if(beams.begin(), beams.end(),
                                 [](const Beam& b) { return !b.active; }), beams.end());
}


void BeamManager::render(SDL_Renderer* renderer, Camera& camera) {
    for (auto& b : beams) b.render(renderer, camera);
}

Vector2D BeamManager::getBeamSpawnPosition(Vector2D& position) const {
    return position + Vector2D(SHIP_SIZE / 2.0f - SHIP_SIZE / 2.0f, SHIP_SIZE / 2.0f);
}

void BeamManager::stopAllBeamSounds() {
    for (auto& beam : beams) {
        if (beam.channel != -1) {
            Mix_HaltChannel(beam.channel);
            beam.channel = -1;
        }
    }
    beams.clear();
}

bool BeamManager::isFiring() const {
    for (const auto& b : beams) {
        return b.active;
    }
    return false;
}
