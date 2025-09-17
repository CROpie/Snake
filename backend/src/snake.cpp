#include "snake.h"

void Snake::grow(Direction dir) {
    Position head = chain.front();
    Position newHead{head.x, head.y};

    switch (dir) {
        case Direction::UP:
            newHead.y --;
            break;
        case Direction::DOWN:
            newHead.y ++;
            break;
        case Direction::LEFT:
            newHead.x --;
            break;
        case Direction::RIGHT:
            newHead.x ++;
            break;
    }
    
    chain.push_front(newHead);
    isGrow = false;
}

void Snake::updateChain(Position newHead) {
    chain.push_front(newHead);
    chain.pop_back();
}

void Snake::move(Direction dir) {

    Position head = chain.front();
    Position newHead{head.x, head.y};

    switch (dir) {
        case Direction::UP:
            // if (pos.y - 1 == -1) break;
            newHead.y --;
            break;
        case Direction::DOWN:
            // if (pos.y + 1 == 11) break;
            newHead.y ++;
            break;
        case Direction::LEFT:
            // if (pos.x - 1 == -1) break;
            newHead.x --;
            break;
        case Direction::RIGHT:
            // if (pos.x - 1 == 11) break;
            newHead.x ++;
            break;
    }
    updateChain(newHead);
}