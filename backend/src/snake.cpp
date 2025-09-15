#include "snake.h"

void Snake::move(Direction dir) {
    switch (dir) {
        case Direction::UP:
            position.y --;
            break;
        case Direction::DOWN:
            position.y ++;
            break;
        case Direction::LEFT:
            position.x --;
            break;
        case Direction::RIGHT:
            position.x ++;
            break;
    }
}