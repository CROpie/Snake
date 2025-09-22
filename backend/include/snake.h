#pragma once

#include <list>
#include <chrono>
#include <thread>

struct Position {
    int x;
    int y;
};

class Snake {
    public:
        enum Direction {
            UP,
            DOWN,
            LEFT,
            RIGHT
        };

        Position head{1,1};
        Position growHead{};
        bool isGrow{false};
        bool isAlive{true};

        std::chrono::steady_clock::time_point respawn_time;

        std::list<Position> chain{head};

        Direction direction{DOWN};

        void move(Direction dir);
        void grow(Direction newHead);
        void updateChain(Position newHead);

        void die();
        void checkRespawn();
};