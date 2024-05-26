#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <set>
std::pair<int, int> getHexFromLine(std::string str);

std::set<std::pair<int, int>> getAliveHexFromLines(std::vector<std::string>);

std::set<std::pair<int, int>> getNextStep(const std::set<std::pair<int, int>>& aliveTiles);

std::vector<std::pair<int ,int>> getNeighbours(std::pair<int ,int>);

#endif