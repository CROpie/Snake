#pragma once

#include <string>
#include <random>
#include "snake.h"

class Player {
    public:
        int client_fd;
        std::string username;

        Snake snake;

        int lives{5};
        int colour;

        Player(int client_fd, std::string username);

    private:

};