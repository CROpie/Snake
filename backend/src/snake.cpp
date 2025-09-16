#include "snake.h"

void Snake::move(Direction dir, Position pos) {
    switch (dir) {
        case Direction::UP:
            if (pos.y - 1 == -1) break;
            position.y --;
            break;
        case Direction::DOWN:
            if (pos.y + 1 == 11) break;
            position.y ++;
            break;
        case Direction::LEFT:
            if (pos.x - 1 == -1) break;
            position.x --;
            break;
        case Direction::RIGHT:
            if (pos.x - 1 == 11) break;
            position.x ++;
            break;
    }
}