#pragma once

#include <string>
#include "snake.h"

class Player {
    public:
        int client_fd;
        std::string username;

        Snake snake;

        int score;

        Player(int client_fd, std::string username);

    private:

};