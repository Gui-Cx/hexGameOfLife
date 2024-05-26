#include "hex.h"
#include "game.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

std::pair<int, int> getHexFromLine(std::string str)
{
    std::pair<int, int> res = {0, 0};
    std::string::iterator it = str.begin();
    std::cout << str << std::endl;
    while (it != str.end())
    {
        if (*it == 'e')
        {
            res = res + std::pair<int, int>{0, 1};
            it++;
        }
        else if (*it == 'w')
        {
            res = res + std::pair<int, int>{0, -1};
            it++;
        } 
        else if (*it == 's' && *(it+1) == 'e')
        {
            res = res + std::pair<int, int>{1, 0};
            it = it+2;
        } 
        else if (*it == 'n' && *(it+1) == 'w')
        {
            res = res + std::pair<int, int>{-1, 0};
            it = it+2;
        }
        else if (*it == 's' && *(it+1) == 'w')
        {
            res = res + std::pair<int ,int>{1, -1};
            it = it+2;
        }
        else if (*it == 'n' && *(it+1) == 'e')
        {
            res = res + std::pair<int, int>{-1, 1};
            it = it+2;
        } else 
        {
            std::cerr << "WTF BRO while processing "<<str<<std::endl;
            return {0,0};
        }
    }
    return res;
}

std::set<std::pair<int, int>> getAliveHexFromLines(std::vector<std::string> lines)
{
    std::set<std::pair<int, int>> res{};
    for (std::string line : lines)
    {
        std::pair<int ,int> p = getHexFromLine(line);
        if (res.find(p) != res.end())
        {
            res.erase(p);
        } else {
            res.insert(p);
        }
    }
    return res;
}

std::set<std::pair<int, int>> getNextStep(const std::set<std::pair<int, int>>& aliveTiles)
{
    std::map<std::pair<int, int>, int> neighbours;
    for (auto hex : aliveTiles)
    {
        for (auto neigh : getNeighbours(hex))
        {
            neighbours[neigh]++;
        }
    }

    std::set<std::pair<int,int>> res;
    for (auto it_neigh = neighbours.begin(); it_neigh != neighbours.end(); it_neigh++)
    {
        if (aliveTiles.find(it_neigh->first) == aliveTiles.end() && it_neigh->second == 2)
        {
            res.insert(it_neigh->first);
        }
        else if (aliveTiles.find(it_neigh->first) != aliveTiles.end() && (it_neigh->second == 1 || it_neigh->second == 2))
        {
            res.insert(it_neigh->first);
        }
    }
    return res;
}

std::vector<std::pair<int ,int>> getNeighbours(const std::pair<int ,int> hex)
{
    std::vector<std::pair<int ,int>> res;
    std::pair<int,int> directions[] = {{0,1}, {1, 0}, {-1, 0}, {0, -1}, {-1, 1}, {1,-1}};
    for (auto dir : directions)
    {
        res.push_back(hex+dir);
    }
    return res;
}