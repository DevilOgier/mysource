#include <iostream>       //std::cout
#include <queue>
#include <functional>


// 子序列和最大值
int greatestSumOfSubarrays(int *data, int len) {
    int max = data[0];
    int currentMax = data[0];
    for (int i = 1; i < len; i++) {
        if (currentMax > 0) {
            currentMax = currentMax + data[i];
        } else {
            currentMax = data[i];
        }
        if (currentMax > max) {
            max = currentMax;
        }
    }
    return max;
}

int main() {
    int data[8] = {1, -2, 3, 10, -4, 7, 2, -5};
    std::cout << greatestSumOfSubarrays(data, 8) << std::endl;
    int data2[4] = {-5, -2, -3, -10};
    std::cout << greatestSumOfSubarrays(data2, 4) << std::endl;
    return 0;
}

// g++ -std=c++11 greatestSumOfSubarrays.cpp -o /home/test && /home/test