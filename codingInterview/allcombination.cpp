#include <iostream>       //std::cout


void allcombination(char *data, int len) {
    if (len <= 0) {
        return;
    }
    int num = 1 << len;
    std::cout << "null" << std::endl;
    for (int i = 1; i < num; i++) {
        for (int j = 0; j < len; j++) {
            if (i & (1 << j)) {
                std::cout << data[j];
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    char data[3] = {'a', 'b', 'c'};
    allcombination(data, 3);
    return 0;
}

// g++ -std=c++11 allcombination.cpp -o /home/test && /home/test