#pragma once

#include "minisocket.hpp"
#include "player.h"

#include "json.hpp"
using json = nlohmann::json;

class GameController {
    public:

        minisocket::MiniSocket& miniSocket;

        std::vector<Player> players;

        GameController(minisocket::MiniSocket& miniSocket);

        void start();

    private:
        void onMessage(int client_fd, const std::string& msg);

        void updateGameState();

        // receiving data from client
        void handleInitialConnect(int client_fd, json request);
        void handleChangeDirection(int client_fd, json request);

        // sending data to client
        json serializeGameState();

        void sendGameStateAll(minisocket::MiniSocket& miniSocket);

        // helpers
        Player* findPlayer(int client_fd);
};