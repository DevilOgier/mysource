#include <iostream>       //std::cout
#include <queue>
#include <functional>

#define K 5
#define Len 10

// 最小K个数，使用大顶堆
int topMin() {
    int data[Len] = {25, 3, 5, 2, 6, 8, 1, 9, 10, 7};
    std::priority_queue<int> pqueue;
    for (int i = 0; i < K; i++) {
        pqueue.push(data[i]);
    }
    for (int i = K; i < Len; i++) {
        if (data[i] < pqueue.top()) {
            pqueue.pop();
            pqueue.push(data[i]);
        }
    }
    while (pqueue.size() != 0) {
        std::cout << pqueue.top() << " ";
        pqueue.pop();
    }
    std::cout << std::endl;
}


// 最大K个数，使用小顶堆
int topMax() {
    int data[Len] = {25, 3, 5, 2, 6, 8, 1, 9, 10, 7};
    std::priority_queue<int, std::vector<int>, std::greater<int> > pqueue;
    for (int i = 0; i < K; i++) {
        pqueue.push(data[i]);
    }
    for (int i = K; i < Len; i++) {
        if (data[i] > pqueue.top()) {
            pqueue.pop();
            pqueue.push(data[i]);
        }
    }
    while (pqueue.size() != 0) {
        std::cout << pqueue.top() << " ";
        pqueue.pop();
    }
    std::cout << std::endl;
}

int main() {
    topMin();
    topMax();
    return 0;
}

// g++ -std=c++11 top_k.cpp -o /home/test && /home/test