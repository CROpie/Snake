#include "player.h"

Player::Player(int client_fd, std::string username)
    : client_fd(client_fd), username(username)
{
    std::mt19937 gen(std::random_device{}());
    colour = std::uniform_int_distribution<int>(0, 5)(gen);
}