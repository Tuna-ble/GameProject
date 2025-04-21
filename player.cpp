#include "player.h"

void Player::init(Graphics& graphics, Audio& sound) {
    playerTexture = graphics.getTexture("spaceShip");
    bullets.init(graphics.getTexture("bullet"));
    thruster.init(graphics.getTexture("thruster"), SHIP_FRAMES, SHIP_CLIPS);
    health = Health(10);
    SFX = &sound;
}

void Player::handleInput(SDL_Event& event, Camera &camera) {
    int mousex, mousey;
    SDL_GetMouseState(&mousex, &mousey);
    Vector2D mouse(static_cast<float>(mousex), static_cast<float>(mousey));

    switch (event.type) {
        case SDL_QUIT:
            gameRunning = false;
            exit(0);
            break;
        case SDL_MOUSEBUTTONDOWN:
            Vector2D worldMouse = mouse + camera.position;
            Vector2D playerCenter = position + Vector2D(SHIP_SIZE / 2.0f, SHIP_SIZE / 2.0f);
            Vector2D direction = worldMouse - playerCenter;
            Vector2D spawn = bullets.getBulletSpawnPosition(position);
            direction = direction.normalize();
            float bulletAngle = atan2(worldMouse.y - spawn.y - BULLET_SIZE / 2, worldMouse.x - spawn.x - BULLET_SIZE / 2) * 180 / M_PI + 90;
            bullets.shoot(spawn, direction, damage, bullets.bulletSpeed, bulletSrcRect, bulletAngle, bulletFrom::PLAYER);

            SFX->playSound("shoot");

            break;
    }

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    velocity = Vector2D(0, 0);

    if (currentKeyStates[SDL_SCANCODE_W]) velocity.y = -speed;
    if (currentKeyStates[SDL_SCANCODE_S]) velocity.y = speed;
    if (currentKeyStates[SDL_SCANCODE_A]) velocity.x = -speed;
    if (currentKeyStates[SDL_SCANCODE_D]) velocity.x = speed;
}

void Player::update(float deltaTime, Camera &camera) {
    position += velocity * deltaTime;

    position.x = std::max(0.f, std::min(position.x, (float)mapWidth - (float)SHIP_SIZE));
    position.y = std::max(0.f, std::min(position.y, (float)mapHeight - (float)SHIP_SIZE));

    int mousex, mousey;
    SDL_GetMouseState(&mousex, &mousey);
    Vector2D worldMouse = Vector2D(static_cast<float>(mousex), static_cast<float>(mousey)) + camera.position;
    Vector2D playerCenter = position + Vector2D(SHIP_SIZE / 2.0f, SHIP_SIZE / 2.0f);
    angle = atan2(worldMouse.y - playerCenter.y, worldMouse.x - playerCenter.x) * 180 / M_PI + 90;

    if (health.getPercent() < 1.0f) healTimer += deltaTime;
    if (healTimer >= healCooldown) {
        health.heal(2);
        healTimer = 0.0f;
    }

    if (hurtTimer > 0.0f) {
        hurtTimer -= deltaTime;
    }

    thruster.update();

    bullets.update(deltaTime);
}

void Player::render(SDL_Renderer* renderer, Camera &camera, int ID) {
    Vector2D draw = position - camera.position;

    if (hurtTimer > 0.0f) {
    draw.x += rand() % 5 - 2;
    draw.y += rand() % 5 - 2;
    }

    SDL_Rect dest = { static_cast<int>(draw.x), static_cast<int>(draw.y), SHIP_SIZE, SHIP_SIZE };

    if (hurtTimer > 0.0f)
    SDL_SetTextureColorMod(playerTexture, 255, 100, 100);
    else
    SDL_SetTextureColorMod(playerTexture, 255, 255, 255);

    thruster.render(renderer, position, camera, SHIP_SIZE, angle);

    SDL_RenderCopyEx(renderer, playerTexture, &srcRect, &dest, angle, NULL, SDL_FLIP_NONE);

    bullets.render(renderer, camera);

    healthBar.render(renderer, health, {20 , 20}, 200, 30);
}

void Player::getBuff(int value, dropType type) {
    switch (type) {
    case dropType::HEALTH:
        if (health.current + value > health.cap) health.current = health.cap;
        else health.current += value;
        break;
    case dropType::DAMAGE:
        damage += value;
        break;
    case dropType::SPEED:
        if (speed + value > 400) speed = 400;
        else speed += value;
        break;
    }
}

void Player::reset() {
    position = {startX, startY};
    velocity = Vector2D(0, 0);
    health = Health(10);
    bullets.bullets.clear();
    speed = 200;
    gameRunning = true;
}
