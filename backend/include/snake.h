#pragma once

class Snake {
    public:
        enum Direction {
            UP,
            DOWN,
            LEFT,
            RIGHT
        };

        struct Position {
            int x;
            int y;
        };

        Direction direction{DOWN};
        Position position{0,0};

        void move(Direction dir, Position pos);
};