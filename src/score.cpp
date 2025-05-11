#include "score.h"

int Score::loadHighScore() {
    std::ifstream file("data/highscore.txt");
    int highScore = 0;
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
    return highScore;
}

void Score::saveHighScore(int score) {
    std::ofstream file("data/highscore.txt");
    if (file.is_open()) {
        file << score;
        file.close();
    }
}
