#include "gameController.h"

GameController::GameController(minisocket::MiniSocket& miniSocket, SnakeGame& snakeGame)
    : miniSocket(miniSocket), snakeGame(snakeGame)
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
    json positionData = json::array();

    for (auto& player : players) {

        json chain = json::array();

        for (auto& pos: player.snake.chain) {
            chain.push_back({
                { "x", pos.x },
                { "y", pos.y }
            });
        }

        json playerData = {
            { "player", player.client_fd },
            { "chain", chain}
        };

        positionData.push_back(playerData);
    }

    // make consistent with players for now, but can improve later
    json powerupChain = json::array();

    powerupChain.push_back({
        { "x", snakeGame.powerup.x },
        { "y", snakeGame.powerup.y }
    });

    json powerupData = {
        { "player", "powerup" },
        { "chain", powerupChain}
    };

    positionData.push_back(powerupData);

    return positionData;
}

void GameController::sendGameStateAll(minisocket::MiniSocket& miniSocket) {
    json gameState = serializeGameState();

    for (auto& player : players) {
        miniSocket.sendFrame(player.client_fd, gameState.dump(2));
    }
}

void GameController::updateGameState() {
    for (Player& player : players) {

        if (player.snake.isGrow) {
            player.snake.grow(player.snake.direction);
        } else {
            player.snake.move(player.snake.direction);
        }

        if (snakeGame.isTouchingPowerup(player)) {
            player.score++;
            snakeGame.movePowerup();
        }
    }
}

void GameController::start() {
    miniSocket.init();

    std::thread([&]() { miniSocket.run(); }).detach();

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(TICK));
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

