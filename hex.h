# ifndef HEX_H
# define HEX_H
#include <iostream>

using hex = std::pair<int, int>;

std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& pair);

std::pair<int, int> operator+(const std::pair<int, int>& a, const std::pair<int, int>& b);

std::pair<int, int> operator-(const std::pair<int, int>& a, const std::pair<int, int>& b);
#endif