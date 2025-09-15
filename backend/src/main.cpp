#include "snake.h"
#include "minisocket.hpp"

#include "json.hpp"
using json = nlohmann::json;

Snake chris;

std::vector<int> players;

void handleMove(std::string dir) {

    std::cout << "Direction: " << dir << std::endl;


    if (dir == "ArrowUp") {
        chris.direction = Snake::UP;
    }
    if (dir == "ArrowDown") {
        chris.direction = Snake::DOWN;
    }
    if (dir == "ArrowLeft") {
        chris.direction = Snake::LEFT;
    }
    if (dir == "ArrowRight") {
        chris.direction = Snake::RIGHT;
    }
}

json serializeGameState() {
    json gameState{
        {"x", chris.position.x},
        {"y", chris.position.y},
        };
    return gameState;
}

void sendGameStateAll(minisocket::MiniSocket& miniSocket) {
    json gameState = serializeGameState();

    for (auto& player : players) {

        miniSocket.sendFrame(player, gameState.dump(2));
    }
}

void updateGameState() {
    chris.move(chris.direction);
    // if (chris.position.y == 10) {
    //     chris.direction = Snake::UP;
    // } else if (chris.position.y == 0) {
    //     chris.direction = Snake::DOWN;
    // }
}

void handleInitialConnect(int client_fd, json request) {
    players.emplace_back(client_fd);
}

void onMessage(int client_fd, const std::string& msg) {
    if (msg.empty()) return;

    json request = json::parse(msg);
    
    std::string action = request["action"];
    if (action == "join") {
        handleInitialConnect(client_fd, request);
    }

    if (action == "move") {
        handleMove(request["direction"]);
    }
}

int main() {

    // library to connect server to client
    minisocket::MiniSocket miniSocket("9023");

    miniSocket.on_message = onMessage;

    miniSocket.init();

    std::thread([&]() { miniSocket.run(); }).detach();

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        updateGameState();
        sendGameStateAll(miniSocket);
    }
}