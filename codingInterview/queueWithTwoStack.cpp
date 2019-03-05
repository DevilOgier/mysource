#include <iostream>       //std::cout
#include <stack>


template<typename T>
class Queue {
public:
    Queue(void);

    ~Queue();

    void pop(void);

    void push(T &node);

    void push(T node);

    T &Top();

    int size();

private:
    std::stack<T> stack1;
    std::stack<T> stack2;
};

template<typename T>
//这里不能少了T
Queue<T>::Queue(void) {}

template<typename T>
Queue<T>::~Queue(void) {}

template<typename T>
void Queue<T>::push(T &node) {
    stack1.push(node);
}

template<typename T>
void Queue<T>::push(T node) {
    stack1.push(node);
}

template<typename T>
void Queue<T>::pop() {
    if (stack2.size() <= 0) {
        while (stack1.size() != 0) {
            stack2.push(stack1.top());
            stack1.pop();
        }
    }
    if (stack2.size() == 0)
        throw "queue is empty";
    stack2.pop();
}

template<typename T>
T &Queue<T>::Top() {
    if (stack2.size() <= 0) {
        while (stack1.size() != 0) {
            stack2.push(stack1.top());
            stack1.pop();
        }
    }
    if (stack2.size() == 0)
        throw "queue is empty";
    return stack2.top();
}

template<typename T>
int Queue<T>::size() {
    return stack1.size() + stack2.size();
}


// 数组去重
int main() {

    Queue<char> queue;
    queue.push('a');
    queue.push('b');
    queue.push('c');

    std::cout << queue.Top() << " ";
    queue.pop();
    queue.push('d');

    while (queue.size() != 0) {
        std::cout << queue.Top() << " ";
        queue.pop();
    }
    std::cout << std::endl;
}

// g++ -std=c++11 queueWithTwoStack.cpp -o /home/test && /home/test