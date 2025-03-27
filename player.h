#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "def.h"
#include "graphics.h"

struct Player{
    Graphics graphics;
    int x = 100, y = 100;
    int vx, vy;
    const int speed = 5;
    bool gameRunning = true;

    void init();
    void renderPlayer(int ID);
    void renderCursor();
    void handleInput();
    void posUpd();
};

#endif // PLAYER_H_INCLUDED
