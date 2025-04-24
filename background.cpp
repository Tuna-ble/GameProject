#include "background.h"

// ==== Camera ====

void Camera::update(Player &player)
{
    position = player.position + Vector2D(SHIP_SIZE / 2, SHIP_SIZE / 2) - Vector2D(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    position.x = std::max(0, std::min((int)position.x, MAP_WIDTH - w));
    position.y = std::max(0, std::min((int)position.y, MAP_HEIGHT - h));
}

// ==== Map ====

void TiledRenderer::init(Graphics& graphics) {
    for (int i = 0; i < 6; i++) {
        std::string name = "background" + std::to_string(i+1);
        backgroundTexture[i] = graphics.getTexture(name.c_str());
        parallaxStrength[i] = baseParallax;
        baseParallax += plusVal;
    }
}

void TiledRenderer::render(SDL_Renderer* renderer, Camera& camera)
{
    for (int i = 0; i < 6; i++) {
    Vector2D scrollPos = camera.position * parallaxStrength[i];

    int tileXStart = (int)std::floor(scrollPos.x / BACKGROUND_SIZE);
    int tileYStart = (int)std::floor(scrollPos.y / BACKGROUND_SIZE);

    int tilesX = SCREEN_WIDTH / BACKGROUND_SIZE + 4;
    int tilesY = SCREEN_HEIGHT / BACKGROUND_SIZE + 4;

    for (int y = -1; y < tilesY - 1; y++) {
        for (int x = -1; x < tilesX - 1; x++) {
            int posX = (tileXStart + x) * BACKGROUND_SIZE - (int)scrollPos.x;
            int posY = (tileYStart + y) * BACKGROUND_SIZE - (int)scrollPos.y;

            SDL_Rect dst = { posX, posY, BACKGROUND_SIZE, BACKGROUND_SIZE };
            SDL_RenderCopy(renderer, backgroundTexture[i], NULL, &dst);
        }
    }
    }
}
