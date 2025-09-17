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

        Position head{0,2};
        Position body{0,1};
        Position tail{0,0};

        Position growHead{};
        bool isGrow{false};

        std::list<Position> chain{tail, body, head};

        Direction direction{DOWN};

        void move(Direction dir);
        void grow(Direction newHead);
        void updateChain(Position newHead);
};