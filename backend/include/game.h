#pragma once

#include "snake.h"
#include "player.h"
#include "map.h"

#include <random>

class SnakeGame {
    public:
        Map map;

        SnakeGame(Map map);

        Position powerup{5,5};

        bool isTouchingPowerup(Player& player);
        bool isTouchingWall(Player& player);
        bool isTouchingSnake(std::vector<Player>& players, Player& player);

        void movePowerup();

    private:
        std::mt19937 gen;

        int getRandom(int N);

};