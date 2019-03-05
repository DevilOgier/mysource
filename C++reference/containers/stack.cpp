/*
http://www.cplusplus.com/reference/stack/stack/

http://classfoo.com/ccby/article/s4SvF  中文版

堆栈stack是容器适配器，它是以序列容器为底层实现。stack支持后入先出(LIFO)，数据的插入和删除都
是在堆栈的顶部进行的，不能够访问堆栈内部的数据。stack不支持迭代器。

template <class T, class Container = deque<T> > class stack;

Element access:
top	访问顶部元素

Capacity:
empty	判断是否为空
size	返回有效元素个数

Modifiers:
push	在容器顶部插入元素
pop	移除容器顶部的元素
emplace C++11	在容器顶部放置插入元素
swap	交换容器的内容


value_type& top();
const value_type& top() const;
bool empty() const;
size_type size() const;

void pop();
void push (const value_type& val);
void push (value_type&& val);
void swap (stack& x)

*/

#include <iostream>       // std::cin, std::cout
#include <stack>          // std::stack
#include <string>         // std::string, std::getline(string)
#include <vector>         // std::vector
#include <deque>          // std::deque

//using namespace std; 

int main ()
{
	//实例化一个堆栈类
	std::stack<std::string> mystack;

	//将数据压入堆栈容器适配器
	mystack.push ("First sentence");
	mystack.push ("Second sentence");

	//返回堆栈容器适配器中的数据个数
	std::cout << "stack size: " << mystack.size() << '\n';

	std::cout << "mystack contains:\n";

	//测试堆栈容器适配器类型是否为空
	while (!mystack.empty())
	{

		//访问堆栈容器适配器栈顶数据
		std::cout << mystack.top() << '\n';
		//弹出堆栈容器适配器栈顶的数据
		mystack.pop();
	}

	//堆栈是容器适配器，它是以序列容器为底层实现.标准的容器类模板vector, deque 和list可以使用，默认情况下，
	//如果没有容器类被指定成为一个特别的stack 类，标准的容器类模板就是deque 队列
	std::deque<int> mydeque (3,100);          // deque with 3 elements {100, 100, 100}
	std::vector<int> myvector (2,200);        // vector with 2 elements {200, 200}

	std::stack<int> first;                    // empty stack
	std::stack<int> second (mydeque);         // stack initialized to copy of deque

	std::stack<int,std::vector<int> > third;  // empty stack using vector
	std::stack<int,std::vector<int> > fourth (myvector);

	/*
	(gdb) print first
	$2 = std::stack wrapping: std::deque with 0 elements
	(gdb) print second
	$3 = std::stack wrapping: std::deque with 3 elements = {100, 100, 100}
	(gdb) print third
	$4 = std::stack wrapping: std::vector of length 0, capacity 0
	(gdb) print fourth
	$5 = std::stack wrapping: std::vector of length 2, capacity 2 = {200, 200}
	*/

	std::cout << "size of first: " << first.size() << '\n';
	std::cout << "size of second: " << second.size() << '\n';
	std::cout << "size of third: " << third.size() << '\n';
	std::cout << "size of fourth: " << fourth.size() << '\n';
	std::cout << "The top of fourth: " << fourth.top() << '\n';
    
    std::stack<int> foo,bar;
    foo.push (10); foo.push(20); foo.push(30);
    bar.push (111); bar.push(222);

    foo.swap(bar);

    std::cout << "size of foo: " << foo.size() << '\n';
    std::cout << "size of bar: " << bar.size() << '\n';
    
    std::stack<int> stest;
    int val = 2;
    stest.push(val);
    val++;
    std::cout << stest.top() << '\n';

    std::vector<int> vtest;
    std::vector<int> vtest2;
    std::stack<std::vector<int>> stest2;
    stest2.push(vtest);
    vtest.push_back(1);
    // 虽然返回值是引用类型，但是是临时变量引用了，然后临时变量拷贝给vtest2
    vtest2 = stest2.top();
    std::cout << vtest.size() << " " << vtest2.size()<< '\n';

    // 引用要在定义时初始化
    std::vector<int> &vtest3 = stest2.top();
    std::vector<int> &vtest4 = stest2.top();
    vtest3.push_back(1);
    std::cout << vtest3.size() << " " << vtest4.size()<< '\n';
    // https://blog.csdn.net/u012501459/article/details/44132147/
    //另外关于 push 为什么会执行拷贝构造函数，push_back 的原型为：
    //void push (value_type&& val);
    // 参数是以引用方式传递，按说不会拷贝，但 push 实际实现中判断空间不足时是调用 insert 函数添加元素：

    std::stack<std::vector<int>> stest3;
    vtest.push_back(2);
    stest3.push(vtest);
    stest3.top();
    stest3.push(vtest);
    std::vector<int> &vtest5 = stest3.top();
    std::cout << vtest.size() << " " << vtest5.size()<< '\n';   // 2 2
    vtest.pop_back();
    std::cout << vtest.size() << " " << vtest5.size()<< '\n';   // 1 2

    std::stack<std::vector<int> *> stest4;
    stest4.push(&vtest);


    std::stack<std::string> mystack2;
    // 跟push有什么区别？
    mystack2.emplace ("First sentence");
    mystack2.emplace ("Second sentence");

    std::cout << "mystack contains:\n";
    while (!mystack2.empty())
    {
        std::cout << mystack2.top() << '\n';
        mystack2.pop();
    }


    return 0;
}
/*
g++ -std=c++11 stack.cpp -o /home/test && /home/test

output:
stack size: 2
mystack contains:
Second sentence
First sentence
size of first: 0
size of second: 3
size of third: 0
size of fourth: 2
size of fourth: 200
*/