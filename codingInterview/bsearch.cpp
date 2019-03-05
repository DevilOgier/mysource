#include <iostream>       //std::cout
#include <cstdlib>
#include <vector>
#include <algorithm>

#define Len 10


int bsearch(int *data, int len, int val) {
    int start = 0;
    int end = len - 1;
    int mid;
    while (start <= end) {
        mid = (end - start) >> 1 + start;
        if (data[mid] > val) {
            end = mid - 1;
        } else if (data[mid] < val) {
            start = mid + 1;
        } else {
            return mid;
        }
    }
    return -1;
}


int compare(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}


int main() {
    int data[Len] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    std::cout << bsearch(data, Len, 50) << std::endl;
    std::cout << bsearch(data, Len, 5) << std::endl;
    int num = 50;
    int *ptr = NULL;
    ptr = (int *) bsearch(&num, data, Len, sizeof(int), compare);
    if (ptr != NULL) {
        std::cout << *ptr << std::endl;
        std::cout << ptr << std::endl << data << std::endl;
        std::cout << "index : " << (int) (ptr - data) << std::endl;
    }
    num = 5;
    ptr = (int *) bsearch(&num, data, Len, sizeof(int), compare);
    if (ptr != NULL) {
        std::cout << *ptr << std::endl;
        std::cout << ptr << std::endl << data << std::endl;
        std::cout << "index : " << (int) (ptr - data) << std::endl;
    }
    return 0;
}

// g++ -std=c++11 bsearch.cpp -o /home/test && /home/test