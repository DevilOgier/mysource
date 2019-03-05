#include <iostream>       //std::cout

int count = 0;

void permutation(char *data, int start, int end, int num) {
    if (start == num) {
        count++;
        for (int i = 0; i < num; i++) {
            std::cout << data[i];
        }
        std::cout << std::endl;
        return;
    }
    for (int i = start; i <= end; i++) {
        std::swap(data[start], data[i]);
        permutation(data, start + 1, end, num);
        std::swap(data[start], data[i]);
    }
}

int main() {
    char data[5] = {'a', 'b', 'c', 'd', 'e'};
    //p(3,5) = 5*4*3
    count = 0;
    permutation(data, 0, 4, 2);
    std::cout << "count :" << count << std::endl;
    return 0;
}

// g++ -std=c++11 permutation.cpp -o /home/test && /home/test