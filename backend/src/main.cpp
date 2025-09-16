#include "snake.h"
#include "minisocket.hpp"
#include "gameController.h"
#include "game.h"

#include "json.hpp"
using json = nlohmann::json;

Snake chris;

std::vector<int> players;

int main() {

    // library to connect server to client
    minisocket::MiniSocket miniSocket("9023");

    SnakeGame snakeGame;

    GameController gameController(miniSocket, snakeGame);
    gameController.start();
}