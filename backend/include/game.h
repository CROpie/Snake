#pragma once

#include "snake.h"
#include "player.h"

#include <random>

class SnakeGame {
    public:

        SnakeGame();

        int MIN_X{0};
        int MAX_X{10};
        int MIN_Y{0};
        int MAX_Y{10};

        Position powerup{5,5};

        bool isTouchingPowerup(Player& player);
        void movePowerup();

    private:
        std::mt19937 gen;

        int getRandom(int N);

};