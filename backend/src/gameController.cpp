#include "gameController.h"

GameController::GameController(minisocket::MiniSocket& miniSocket, SnakeGame& snakeGame)
    : miniSocket(miniSocket), snakeGame(snakeGame)
{
        miniSocket.on_message = [&](int client_fd, const std::string& msg) {
            this->onMessage(client_fd, msg);
        };

        miniSocket.on_disconnect = [&](int client_fd) {
            this->onDisconnect(client_fd);
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

void GameController::sendGameStateAll() {
    json gameState = serializeGameState();
    json state = {
        { "type", "gameState" },
        { "gameState", gameState }
    };

    for (auto& player : players) {
        state.push_back({"colour", player.colour});
        miniSocket.sendFrame(player.client_fd, state.dump(2));
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

        if (snakeGame.isTouchingWall(player)) {
            player.snake.reset();
            return;
        }

        if (snakeGame.isTouchingSnake(players, player)) {
            player.snake.reset();
            return;
        }
    }
}

void GameController::sendMapState(Player& player) {
    json walls = json::array();

    for (Position& wall : snakeGame.map.walls) {
        walls.push_back({
            { "x", wall.x },
            { "y", wall.y }
        });
    }

    json state = {
        { "type", "walls" },
        { "walls", walls }
    };

    // std::cout << state.dump(2) << std::endl;
    
    miniSocket.sendFrame(player.client_fd, state.dump(2));
}

void GameController::start() {
    miniSocket.init();

    std::thread([&]() { miniSocket.run(); }).detach();

    

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(TICK));
        updateGameState();
        sendGameStateAll();
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
    sendMapState(newPlayer);
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

void GameController::onDisconnect(int client_fd) {

    Player* playerToRemove = findPlayer(client_fd);

    // remove from gameController
    players.erase(
        std::remove_if(players.begin(), players.end(),
            [client_fd](const Player& p) { return p.client_fd == client_fd; }),
        players.end()
    );

    // remove from miniSocket
    miniSocket.remove_client(client_fd);
}