#include <iostream>       //std::cout

int count = 0;

void combination(char *data, int start, int len, char *perfix, int plen, int num) {
    if (plen == num) {
        count++;
        for (int i = 0; i < plen; i++) {
            std::cout << perfix[i];
        }
        std::cout << std::endl;
        return;
    }
    for (int i = start; i <= len - (num-plen); i++) {
        perfix[plen] = data[i];
        combination(data, i + 1, len, perfix, plen + 1, num);
    }

}

int main() {
    char data[5] = {'a', 'b', 'c', 'd', 'e'};
    char perfix[5];
    //c(3,5) = 5*4*3/(1*2*3) = 10
    count = 0;
    combination(data, 0, 5, perfix, 0, 5);
    std::cout << "count :" << count << std::endl;
    return 0;
}

// g++ -std=c++11 combination.cpp -o /home/test && /home/test