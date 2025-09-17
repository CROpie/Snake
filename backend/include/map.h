#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include "snake.h" // just for position, move it later

class Map {
    public:
        std::string filename;
        // maybe split boundary and inner walls? 
        std::vector<Position> walls;
        int height{0};
        int width{0};

        Map(std::string filename);
};