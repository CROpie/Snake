#include "game.h"
#include "player.h"

SnakeGame::SnakeGame() 
    : gen(std::random_device{}())
{}

bool SnakeGame::isTouchingPowerup(Player& player) {
    Position head = player.snake.chain.front();
    if (head.x == powerup.x && head.y == powerup.y) {
        player.snake.isGrow = true;
        return true;
    }

    return false;
}

int SnakeGame::getRandom(int N) {

    std::uniform_int_distribution<> dist(1, N);

    return dist(gen);
}

void SnakeGame::movePowerup() {
    powerup.x = getRandom(10);
    powerup.y = getRandom(10);
}