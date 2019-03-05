#include <iostream>       //std::cout
#include <bitset>
#include <vector>

#define Max 100
#define Len 14

// 数组去重
int main() {
    int data[Len] = {25, 3, 5, 2, 6, 8, 1, 9, 10, 7, 7, 25, 5, 2};
    std::vector<int> data2;
    std::bitset<Max> bset;
    std::vector<int>::const_iterator cit;
    for (int i=0;i<Len;i++){
       if (!bset.test(data[i])){
            data2.push_back(data[i]);
           bset.set(data[i]);
        }
    }
    for (cit = data2.cbegin();cit!=data2.cend();++cit){
        std::cout<< *cit<<" ";
    }
    std::cout<< std::endl;
    return 0;
}

// g++ -std=c++11 bitset.cpp -o /home/test && /home/test