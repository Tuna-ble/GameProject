#include <SDL.h>
#include <SDL_image.h>
#include "game.h"

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
    Game game;
    game.init();
    //SDL_ShowCursor(0);

    /*SDL_Rect rect;
    rect.x=100;
    rect.y=100;
    rect.h=100;
    rect.w=100;
    SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 0 );
    SDL_RenderFillRect(game.renderer, &rect);
    SDL_RenderPresent(game.renderer);*/

    game.run();
    return 0;
}
