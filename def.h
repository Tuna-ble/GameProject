#ifndef _DEFS__H
#define _DEFS__H
#include<bits/stdc++.h>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;
const int SHIP_SIZE = 75;
const int ENEMY_SIZE = 60;
const int BOSS_SIZE = 75;
const int BULLET_SIZE = 30;
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;
const int backgroundSize = 512;
const int mapWidth = 1800;
const int mapHeight = 1400;
const int ID = 0;
const int THRUSTER_CLIPS[][4] = {
    {  0, 512, 256, 512},
    { 256, 512, 256, 512},
    {512, 512, 256, 512},
    {768, 512, 256, 512}};
    const int THRUSTER_FRAMES = sizeof(THRUSTER_CLIPS)/sizeof(int)/4;

#define WINDOW_TITLE "Space War"

#endif
