#ifndef _DEFS__H
#define _DEFS__H
#include<bits/stdc++.h>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;
const int SHIP_SIZE = 75;
const int ENEMY_SIZE = 60;
const int ASTEROID_SIZE = 100;
const int BULLET_SIZE = 30;
const int DROP_SIZE = 30;
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;
const int backgroundSize = 512;
const int mapWidth = 1800;
const int mapHeight = 1400;
const int ID = 1;
const int playTime = 60;
const int EXPLOSION_CLIPS[][4] = {
    { 0, 32, 32, 32}, { 32, 32, 32, 32 },
    { 64, 32, 32, 32}, { 96, 32, 32, 32},
    { 128, 32, 32, 32}, { 160, 32, 32, 32},
    { 192, 32, 32, 32}, { 224, 32, 32, 32}
    };
const int EXPLOSION_FRAMES = sizeof(EXPLOSION_CLIPS)/sizeof(int)/4;

const int SHIP_CLIPS[][4] = {
    { 0, 0, 36, 36}, { 36, 0, 36, 36},
    { 72, 0, 36, 36}, { 108, 0, 36, 36},
    { 0, 36, 36, 36}, { 36, 36, 36, 36},
    { 72, 36, 36, 36}, { 108, 36, 36, 36}
    };
const int SHIP_FRAMES = sizeof(SHIP_CLIPS)/sizeof(SHIP_CLIPS[0]);

const int startX = 100, startY = 100;

#define WINDOW_TITLE "Space War"

#endif
