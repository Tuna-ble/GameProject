#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED
#include "def.h"

struct Score {
    int loadHighScore();
    void saveHighScore(int score);
};

#endif // SCORE_H_INCLUDED
