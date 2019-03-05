/*
http://www.cplusplus.com/reference/vector/

http://classfoo.com/ccby/article/jnevK  中文版

向量（Vector）是一个封装了动态大小数组的顺序容器（Sequence container）。跟任意其它类型容器一样，
 它能够存放各种类型的对象。可以简单的认为，向量是一个能够存放任意类型的动态数组。

template < class T, class Alloc = allocator<T> > class vector;

成员函数
(constructor)	创建 vector
(destructor)	释放 vector
operator=	值赋操作

​Iterators:
begin	返回指向容器起始位置的迭代器（iterator）
end	返回指向容器末尾位置的迭代器
rbegin	返回指向容器逆序起始位置的逆序迭代器（reverse_iterator）
rend	返回指向容器逆序末尾位置的逆序迭代器
cbegin C++11	返回指向容器起始位置的常迭代器（const_iterator）
cend C++11	返回指向容器末尾位置的常迭代器
crbegin C++11	返回指向容器逆序起始位置的常逆序迭代器（const_reverse_iterator）
crend C++11	返回指向容器逆序末尾位置的常逆序迭代器

 Capacity:
size	返回有效元素个数
max_size	返回 vector 支持的最大元素个数
resize	改变有效元素的个数
capacity	返回当前可使用的最大元素内存块数（即存储容量）
empty	判断是否为空
reserve	请求改变存储容量
shrink_to_fit C++11	请求移除未使用的存储空间

Element access:
operator[]	访问元素
at	访问元素
front	访问第一个元素
back	访问最后一个元素
data C++11	返回当前向量内部数组的指针

Modifiers:
assign	将新的内容赋值给容器
push_back	在末尾增加一个元素
pop_back	删除最后一个元素
insert	插入元素
erase	删除元素
swap	交换内容
clear	清空内容
emplace C++11	构造及插入一个元素
emplace_back C++11	在容器末尾构造及插入一个元素

 Allocator:
get_allocator	获得内存分配器

*/

#include <iostream>
#include <vector>
#include <algorithm> // sort, max_element, random_shuffle, remove_if, lower_bound
#include <functional> // greater, bind2nd

// 用在此处是为了方便简洁, 在实际编程中慎用
//using namespace std;

int main()
{
    int arr[4] = {1, 2, 3, 4};
    // 用上述数组初始化向量
    std::vector<int> foo(arr, arr+4);

    // 插入更多的元素
    foo.push_back(5);
    foo.push_back(6);
    foo.push_back(7);
    foo.push_back(8);

    // 此时的向量内容为 {1, 2, 3, 4, 5, 6, 7, 8}

    std::cout << "size: " << foo.size() << " max_size: " << foo.max_size() << " capacity: " << foo.capacity()<< "\n";
    foo.resize(foo.size()/2);
    foo.resize(foo.size()*2);

    // 随机移动元素
    random_shuffle( foo.begin(), foo.end() );

    // 定位最大的元素, O(n)
    std::vector<int>::const_iterator largest =
            max_element( foo.begin(), foo.end() );

    std::cout << "当前最大元素是： " << *largest << "\n";
    std::cout << "它的索引位置是： " << largest - foo.begin() << "\n";

    // 排序元素
    sort( foo.begin(), foo.end() );

    // 用二分查找法找出向量中值为5的元素
    std::vector<int>::const_iterator five =
            lower_bound( foo.begin(), foo.end(), 5 );

    std::cout << "值为5的元素的索引位置是： " << five - foo.begin() << "\n";

    // 删除所有值大于4的元素
    foo.erase( remove_if(foo.begin(), foo.end(),
                         bind2nd(std::greater<int>(), 4) ), foo.end() );

    // 打印所有剩余元素的值
    for(std::vector<int>::const_iterator it = foo.begin(); it != foo.end(); ++it)
    {
        std::cout << *it << " ";
    }

    std::cout << "\n";
    return 0;
}

/*
g++ -std=c++11 vector.cpp -o /home/test && /home/test


*/