#include "game.h"
#include "player.h"

SnakeGame::SnakeGame() 
    : gen(std::random_device{}())
{}

bool SnakeGame::isTouchingPowerup(Player& player) {
    if (player.snake.position.x == powerup.x && player.snake.position.y == powerup.y) return true;

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