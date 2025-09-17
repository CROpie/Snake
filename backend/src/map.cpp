#include "map.h"

Map::Map(std::string filename)
    : filename(filename)
{
    std::string path = "../maps/" + filename;
    std::ifstream infile(path);
    if (!infile) throw std::runtime_error("Could not open " + filename);

    std::vector<std::string> grid;
    std::string line;

    while (std::getline(infile, line)) {
        grid.push_back(line);
    }

    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {
            if (grid[y][x] == 'X') {
                walls.push_back({x, y});
            }
        }
    }

    height = grid.size();
    width = grid[0].size();
}