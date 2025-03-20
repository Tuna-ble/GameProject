#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "def.h"
#include "graphics.h"

using namespace std;

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_PollEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}


int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.init();
    SDL_ShowCursor(0);

    SDL_Texture* background = graphics.loadTexture("bikiniBottom.jpg");
    //graphics.prepareScene(background);

    //graphics.presentScene();
    //waitUntilKeyPressed();

    SDL_Texture* spongeBob = graphics.loadTexture("Spongebob.png");
    //graphics.renderTexture(spongeBob, 200, 200);

    //graphics.presentScene();
    //waitUntilKeyPressed();

    /*SDL_DestroyTexture( spongeBob );
    spongeBob = NULL;
    SDL_DestroyTexture( background );
    background = NULL;*/

    SDL_Rect rect;
    rect.x=100;
    rect.y=100;
    rect.h=100;
    rect.w=100;
    SDL_SetRenderDrawColor(graphics.renderer, 255, 255, 255, 0 );
    SDL_RenderFillRect(graphics.renderer, &rect);
    SDL_RenderPresent(graphics.renderer);

    SDL_Texture *targetterTexture = graphics.loadTexture("cursor.png");
    SDL_Event event;
    int x, y;
    while (true) {
        SDL_GetMouseState(&x, &y);
        //cerr << ((x > 100 && y > 100 && x < 200 && y < 200) ? "In\n" : "Out\n");

        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                 exit(0);
                 break;
            case SDL_MOUSEBUTTONDOWN:
                 cerr << "Down at (" << x << ", " << y << ")\n";
                 break;
            case SDL_MOUSEBUTTONUP:
                 cerr << "Up at (" << x << ", " << y << ")\n";
                 break;

        }

        SDL_RenderClear(graphics.renderer);
        graphics.prepareScene(background);
        graphics.renderTexture(spongeBob, 200, 200);

        SDL_Rect cursorRect = { x-16, y-16, 32, 32 }; // Đặt kích thước mong muốn, ví dụ 32x32
        SDL_RenderCopy(graphics.renderer, targetterTexture, NULL, &cursorRect);
        SDL_RenderPresent(graphics.renderer);

        SDL_Delay(16);
    }

    SDL_DestroyTexture( spongeBob );
    spongeBob = NULL;
    SDL_DestroyTexture( background );
    background = NULL;

    graphics.quit();
    return 0;
}



