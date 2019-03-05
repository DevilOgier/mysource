#include <iostream>       //std::cout
#include <unordered_map>
#include <vector>

#define Max 100
#define Len 14

// 数组去重
int main() {
    int data[Len] = {25, 3, 5, 2, 6, 8, 1, 9, 10, 7, 7, 25, 5, 2};
    std::vector<int> data2;
    std::unordered_map<int, bool> map;
    std::vector<int>::const_iterator cit;
    for (int i = 0; i < Len; i++) {
        if (!map[data[i]]) {
            data2.push_back(data[i]);
            map[data[i]] = true;
        }
    }
    for (cit = data2.cbegin(); cit != data2.cend(); ++cit) {
        std::cout << *cit << " ";
    }
    std::cout << std::endl;
    return 0;
}

// g++ -std=c++11 hashmap.cpp -o /home/test && /home/test