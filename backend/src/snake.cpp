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
    updateChain(newHead);
}

void Snake::reset() {
    chain.clear();

    Position head{1,1};
    chain.push_back(head);
    direction = DOWN;
}