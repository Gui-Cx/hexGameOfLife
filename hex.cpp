#include "hex.h"

std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& pair)
{
    os << '{'<< pair.first << ',' << pair.second << '}' ;
    return os;
}

std::pair<int, int> operator+(const std::pair<int, int>& a, const std::pair<int, int>& b)
{
    return std::pair<int, int>{a.first + b.first, a.second + b.second};
}

std::pair<int, int> operator-(const std::pair<int, int>& a, const std::pair<int, int>& b)
{
    return std::pair<int, int>{a.first - b.first, a.second - b.second};
}