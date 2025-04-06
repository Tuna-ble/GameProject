#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_image.h>
#include "def.h"
#include "background.h"
#include "player.h"

SDL_Rect Camera::getViewRect() const
{
    return {(int)x, (int)y, w, h};
}

void Camera::updateCamera(Player &player)
{
    x = player.x - w / 2;
    y = player.y - h / 2;

    if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x > w)
		x = w;
	if (y > h)
		y = h;
}

void TiledRenderer::renderer(SDL_Renderer* renderer, SDL_Texture* texture, Camera& camera)
{
    float paralaxOffsetX = -camera.x * paralaxStrength;
    float paralaxOffsetY = -camera.y * paralaxStrength;

    int tileXStart = (int)((camera.x - paralaxOffsetX) / backgroundSize);
    int tileYStart = (int)((camera.y - paralaxOffsetY) / backgroundSize);

    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            int posX = (tileXStart + x) * backgroundSize + (int)paralaxOffsetX;
            int posY = (tileYStart + y) * backgroundSize + (int)paralaxOffsetY;

            SDL_Rect dst = { posX, posY, backgroundSize, backgroundSize };
            SDL_RenderCopy(renderer, texture, NULL, &dst);
        }
    }
}
