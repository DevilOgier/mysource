#include <iostream>       //std::cout
#include <cstdlib>
#include <vector>
#include <algorithm>

#define Len 10

int rand_a_b(int a, int b) {
    if (a == b) {
        return a;
    }
    return rand() % (b - a) + a;
}


void qsort(int *data, int pstart, int pend) {
    if (pstart >= pend) {
        return;
    }
    std::swap(data[pstart], data[rand_a_b(pstart, pend)]);
    int start = pstart;
    int end = pend;
    int temp = data[start];
    while (start < end) {
        while (data[end] > temp && start < end) {
            end--;
        }
        data[start] = data[end];
        while (data[start] < temp && start < end) {
            start++;
        }
        data[end] = data[start];
    }
    data[start] = temp;
    qsort(data, pstart, start - 1);
    qsort(data, start + 1, pend);
}

void print(int *data, int len) {
    for (int i = 0; i < len; i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

int compare(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}


int main() {
    int data[Len] = {25, 3, 5, 2, 6, 8, 1, 9, 10, 7};
    qsort(data, 0, Len - 1);
    print(data, Len);
    int data2[Len] = {25, 3, 5, 2, 6, 8, 1, 9, 10, 7};
    qsort(data2, Len, sizeof(int), compare);
    print(data2, Len);

    int data3[Len] = {25, 3, 5, 2, 6, 8, 1, 9, 10, 7};

    std::vector<int> v(data3, data3 + sizeof(data3) / sizeof(int));
    std::sort(v.begin(), v.end());
    std::vector<int>::iterator it;
    for (it = v.begin(); it != v.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;


    int* p = v.data();
    print(p, Len);
    return 0;
}

// g++ -std=c++11 qsort.cpp -o /home/test && /home/test