#pragma once

#include <list>

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

        std::list<Position> chain{head};

        Direction direction{DOWN};

        void move(Direction dir);
        void grow(Direction newHead);
        void updateChain(Position newHead);

        void reset();
};