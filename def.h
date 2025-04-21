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
const int backgroundSize = 1500;
const int mapWidth = 1800;
const int mapHeight = 1400;
const int ID = 1;
const int playTime = 60;
const int playerHealth = 4;

const int EXPLOSION_CLIPS[][4] = {
    { 0, 0, 24, 24}, { 24, 0, 24, 24 },
    { 48, 0, 24, 24}, { 72, 0, 24, 24},
    { 96, 0, 24, 24}, { 120, 0, 24, 24},
    { 144, 0, 24, 24}, { 168, 0, 24, 24}
    };
const int EXPLOSION_FRAMES = sizeof(EXPLOSION_CLIPS)/sizeof(EXPLOSION_CLIPS[0]);

const int ASEXPLOSION_CLIPS[][4] = {
    { 0, 0, 96, 96}, { 96, 0, 96, 96 },
    { 192, 0, 96, 96}, { 288, 0, 96, 96},
    { 384, 0, 96, 96}, { 480, 0, 96, 96},
    { 576, 0, 96, 96}, { 672, 0, 96, 96}
    };
const int ASEXPLOSION_FRAMES = sizeof(ASEXPLOSION_CLIPS)/sizeof(ASEXPLOSION_CLIPS[0]);

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
