#include "gameController.h"

GameController::GameController(minisocket::MiniSocket& miniSocket)
    : miniSocket(miniSocket)
{
        miniSocket.on_message = [&](int client_fd, const std::string& msg) {
            this->onMessage(client_fd, msg);
        };
}

Player* GameController::findPlayer(int client_fd) {
    auto it = std::find_if(players.begin(), players.end(),
        [client_fd](const Player& p) {
            return p.client_fd == client_fd;
        });

    return (it != players.end()) ? &(*it) : nullptr;
}

json GameController::serializeGameState() {
    json playerPositions = json::array();

    for (auto& player : players) {
        playerPositions.push_back({
            { "player", player.client_fd },
            { "x", player.snake.position.x },
            { "y", player.snake.position.y }
        });
    }

    return playerPositions;
}

void GameController::sendGameStateAll(minisocket::MiniSocket& miniSocket) {
    json gameState = serializeGameState();

    for (auto& player : players) {
        miniSocket.sendFrame(player.client_fd, gameState.dump(2));
    }
}

void GameController::updateGameState() {
    for (auto& player : players) {
        player.snake.move(player.snake.direction, player.snake.position);
    }
}

void GameController::start() {
    miniSocket.init();

    std::thread([&]() { miniSocket.run(); }).detach();

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        updateGameState();
        sendGameStateAll(miniSocket);
    }
}

void GameController::handleChangeDirection(int client_fd, json request) {

    Player* player = findPlayer(client_fd);

    std::string dir = request.value("direction", "");

    if (dir == "ArrowUp") {
        player->snake.direction = Snake::UP;
    }
    if (dir == "ArrowDown") {
        player->snake.direction = Snake::DOWN;
    }
    if (dir == "ArrowLeft") {
        player->snake.direction = Snake::LEFT;
    }
    if (dir == "ArrowRight") {
        player->snake.direction = Snake::RIGHT;
    }
}

void GameController::handleInitialConnect(int client_fd, json request) {
    Player newPlayer(client_fd, request["username"]);
    players.emplace_back(newPlayer);
}

void GameController::onMessage(int client_fd, const std::string& msg) {
    if (msg.empty()) return;

    json request = json::parse(msg);
    
    std::string action = request["action"];
    if (action == "join") {
        handleInitialConnect(client_fd, request);
    }

    if (action == "move") {
        handleChangeDirection(client_fd, request);
    }
}

