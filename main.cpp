#include "readFile.h"
#include "game.h"
#include "hex.h"
#include "visu.h"
#include <iostream>
// #include <

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
    // for (int i = 0; i <= 100; i++)
    // {
    //     std::cout << i <<" steps : "<< aliveHexes.size() <<" tiles alive right now" <<std::endl;
    //     aliveHexes = getNextStep(aliveHexes);
    // }

    draw(aliveHexes);
    return 0;
}