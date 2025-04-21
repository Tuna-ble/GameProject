#include "asteroid.h"

// ==== Asteroid ====

Asteroid::Asteroid(Vector2D position, Vector2D velocity, SDL_Texture* texture, SDL_Rect dest, Audio& sound) :
    position(position), velocity(velocity), texture(texture), dest(dest), SFX(sound), active(true) {}

void Asteroid::render(SDL_Renderer* renderer, SDL_Texture* texture, Camera &camera) {
    Vector2D draw = position - camera.position;
    SDL_Rect dstRect = { static_cast<int>(draw.x), static_cast<int>(draw.y), ASTEROID_SIZE, ASTEROID_SIZE};

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
}

void Asteroid::update(float deltaTime) {
    if (!active) return;

    position += velocity * deltaTime;
    activeTimer += deltaTime;

    if (activeTimer >= activeDuration) {
        active = false;
    }
}

// ==== Asteroid Manager ====

void AsteroidManager::init(Graphics& graphics, Audio& sound) {
    asteroidTexture = graphics.getTexture("asteroid");
    SFX = sound;
    spawnCooldown = float(3 + rand() % 7);
    speed = 50 + rand() % 50;
}

void AsteroidManager::resetSpawnTimer() {
    spawnTimer = 0.0f;
}

bool AsteroidManager::spawnON() {
    return spawnTimer >= spawnCooldown;
}

Vector2D AsteroidManager::spawnOutsideCamera(Camera& camera, int margin) {
    float x, y;
    int side = rand() % 4;

    switch(side) {
        case 0:
            x = camera.position.x + rand() % SCREEN_WIDTH;
            y = camera.position.y - margin;
            break;
        case 1:
            x = camera.position.x + rand() % SCREEN_WIDTH;
            y = camera.position.y + SCREEN_HEIGHT + margin;
            break;
        case 2:
            x = camera.position.x - margin;
            y = camera.position.y + rand() % SCREEN_HEIGHT;
            break;
        case 3:
            x = camera.position.x + SCREEN_WIDTH + margin;
            y = camera.position.y + rand() % SCREEN_HEIGHT;
            break;
    }
    return Vector2D(x, y);
}

void AsteroidManager::spawn(Camera& camera) {
    if (spawnON()) {
        Vector2D spawn = spawnOutsideCamera(camera, 50);
        SDL_Rect dest = { static_cast<int>(spawn.x), static_cast<int>(spawn.y), ASTEROID_SIZE, ASTEROID_SIZE};

        float angle = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;
        Vector2D direction = Vector2D(cos(angle), sin(angle));

        asteroids.emplace_back(spawn, direction * speed, asteroidTexture, dest, SFX);

        resetSpawnTimer();
    }
}

void AsteroidManager::update(float deltaTime) {
    for (auto& a : asteroids) {
        a.update(deltaTime);
        }
    spawnTimer += deltaTime;
    asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(),
                                     [](const Asteroid& a) { return !a.active; }), asteroids.end());
}

void AsteroidManager::render(SDL_Renderer* renderer, Camera &camera) {
    for (auto& a : asteroids) {
        a.render(renderer, asteroidTexture, camera);
    }
}

void AsteroidManager::reset() {
    asteroids.clear();
    spawnTimer = 0;
}
