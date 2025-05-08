#include "player.h"
#include "beam.h"

void Player::init(Graphics& graphics, Audio& sound) {
    playerTexture = graphics.getTexture("spaceShip");
    bullets.init(graphics.getTexture("bullet"));
    beams.init(graphics.getTexture("beam-active0"), sound);
    thruster.init(graphics.getTexture("thruster"), SHIP_FRAMES, SHIP_CLIPS);
    shield.init(graphics.getTexture("shield"));

    health = Health(PLAYER_HEALTH);
    SFX = &sound;

    startX = rand() % MAP_WIDTH;
    startY = rand() % MAP_HEIGHT;
    position = {(float)startX, (float)startY};
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

    if (currentKeyStates[SDL_SCANCODE_E]) {
        if (beamTimer <= 0.0f) {
        Vector2D worldMouse = mouse + camera.position;
        Vector2D playerCenter = position + Vector2D(SHIP_SIZE / 2.0f, SHIP_SIZE / 2.0f);
        Vector2D direction = worldMouse - playerCenter;
        direction = direction.normalize();
        float angle = atan2(worldMouse.y - position.y, worldMouse.x - position.x) * 180 / M_PI + 90;
        beams.shoot(position, damage, angle, true, bulletFrom::PLAYER);

        beamTimer = beamCooldown;
        }
    }
    if (currentKeyStates[SDL_SCANCODE_Q]) {
        shield.activate();
    }
}

void Player::update(float deltaTime, Camera &camera) {
    position += velocity * deltaTime;

    position.x = std::max(0, std::min((int)position.x, MAP_WIDTH - SHIP_SIZE));
    position.y = std::max(0, std::min((int)position.y, MAP_HEIGHT - SHIP_SIZE));

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

    if (beamDamageTimer > 0.0f) {
        beamDamageTimer -= deltaTime;
    }

    if (beamTimer > 0.0f) {
        beamTimer -= deltaTime;
    }

    thruster.update();

    bullets.update(deltaTime);

    shield.update(deltaTime);
    beams.update(deltaTime, position, angle);
}

void Player::render(SDL_Renderer* renderer, Camera &camera) {
    Vector2D draw = position - camera.position;

    if (hurtTimer > 0.0f) {
    draw.x += rand() % 5 - 2;
    draw.y += rand() % 5 - 2;
    }

    SDL_Rect dest = {
        static_cast<int>(draw.x),
        static_cast<int>(draw.y),
        SHIP_SIZE, SHIP_SIZE
    };

    if (hurtTimer > 0.0f)
    SDL_SetTextureColorMod(playerTexture, 255, 100, 100);
    else
    SDL_SetTextureColorMod(playerTexture, 255, 255, 255);

    thruster.render(renderer, position, camera, dest, angle, NULL);

    bullets.render(renderer, camera);
    beams.render(renderer, camera);

    SDL_RenderCopyEx(renderer, playerTexture, &srcRect, &dest, angle, NULL, SDL_FLIP_NONE);

    healthBar.render(renderer, health, {250 , 20}, 400, 30);

    shield.render(renderer, dest);
}

void Player::getBuff(int value, dropType type) {
    switch (type) {
    case dropType::HEALTH:
        health.heal(value);
        break;
    case dropType::DAMAGE:
        if (damage + value > DAMAGE_CAP) damage = DAMAGE_CAP;
        damage += value;
        break;
    case dropType::SPEED:
        if (speed + value > SPEED_CAP) speed = SPEED_CAP;
        else speed += value;
        break;
    }
}

void Player::reset() {
    startX = rand() % MAP_WIDTH;
    startY = rand() % MAP_HEIGHT;
    position = {(float)startX, (float)startY};
    velocity = Vector2D(0, 0);
    beamTimer = 0.0f;
    health = Health(PLAYER_HEALTH);
    bullets.bullets.clear();
    shield.reset();
    speed = BASE_SPEED;

    gameRunning = true;
}
