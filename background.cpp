#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "background.h"
#include "player.h"

//SDL_Rect Camera::getViewRect() const
//{
//    return {(int)x, (int)y, w, h};
//}

void Camera::update(Player &player)
{
    x = player.x + SHIP_SIZE / 2 - SCREEN_WIDTH / 2;
    y = player.y + SHIP_SIZE / 2 - SCREEN_HEIGHT / 2;

    x = std::max(0, std::min((int)x, mapWidth - w));
    y = std::max(0, std::min((int)y, mapHeight - h));
}

void TiledRenderer::render(SDL_Renderer* renderer, SDL_Texture* texture, Camera& camera)
{
    float parallaxOffsetX = -camera.x * parallaxStrength;
    float parallaxOffsetY = -camera.y * parallaxStrength;

    // Dùng floor để tránh mất tile do làm tròn sai
    int tileXStart = (int)std::floor((camera.x - parallaxOffsetX) / backgroundSize);
    int tileYStart = (int)std::floor((camera.y - parallaxOffsetY) / backgroundSize);

    int tilesX = SCREEN_WIDTH / backgroundSize + 3; // +1 để lấp kín, +2 để tránh gap khi lệch
    int tilesY = SCREEN_HEIGHT / backgroundSize + 3;

    for (int y = -1; y < tilesY - 1; y++) {
        for (int x = -1; x < tilesX - 1; x++) {
            int posX = (tileXStart + x) * backgroundSize + (int)parallaxOffsetX;
            int posY = (tileYStart + y) * backgroundSize + (int)parallaxOffsetY;

            SDL_Rect dst = { posX, posY, backgroundSize, backgroundSize };
            SDL_RenderCopy(renderer, texture, NULL, &dst);
        }
    }
}
