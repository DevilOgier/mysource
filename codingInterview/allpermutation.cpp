#include <iostream>       //std::cout


void allpermutation(char *data, int start, int end) {
    if (start == end) {
        for (int i = 0; i <= end; i++) {
            std::cout << data[i];
        }
        std::cout << std::endl;
        return;
    }
    for (int i = start; i <= end; i++) {
        std::swap(data[start], data[i]);
        allpermutation(data, start + 1, end);
        std::swap(data[start], data[i]);
    }
}

int main() {
    char data[3]  = {'a','b','c'};
    allpermutation(data, 0, 2);
    return 0;
}

// g++ -std=c++11 allpermutation.cpp -o /home/test && /home/test