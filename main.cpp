#include "readFile.h"
#include "game.h"
#include "hex.h"
#include "visu.h"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Please give a file to read" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    std::vector<std::string> lines = readFile(filePath);

    std::set<std::pair<int, int>> aliveHexes = getAliveHexFromLines(lines);

    draw(aliveHexes);
    return 0;
}