#include "game.h"
#include "player.h"

SnakeGame::SnakeGame(Map map) 
    : map(map), gen(std::random_device{}())
{}

bool SnakeGame::isTouchingPowerup(Player& player) {
    Position head = player.snake.chain.front();
    if (head.x == powerup.x && head.y == powerup.y) {
        player.snake.isGrow = true;
        return true;
    }

    return false;
}

bool SnakeGame::isTouchingWall(Player& player) {
    Position head = player.snake.chain.front();
    for (Position& wall : map.walls) {
        if (head.x == wall.x && head.y == wall.y) {
            return true;
        }
    }

    return false;
}

int SnakeGame::getRandom(int N) {

    std::uniform_int_distribution<> dist(1, N);

    return dist(gen);
}

void SnakeGame::movePowerup() {
    powerup.x = getRandom(map.width - 2);
    powerup.y = getRandom(map.height - 2);
}