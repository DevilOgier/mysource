//
// Created by Administrator on 2016/12/3.
//
// C++引用详解 http://www.cnblogs.com/gw811/archive/2012/10/20/2732687.html
#include <iostream>

using namespace std;


int fun1() {
    int a = 5;
    int &b = a;
    b = 6;
    cout << "a=" << a << ",b=" << b << endl;//a=6,b=6
    int c = 7;
    b = c;
    cout << "a=" << a << ",b=" << b << endl;//a=7,b=7
    return 0;
}


void fun2() {
    int a = 1;
    int &b = a;
    b = 2;
    cout << "a=" << a << endl; //2
    int c = 1;
    // 常引用
    const int &d = c;
//    d=2;//编译错误 error C2166: l-value specifies const object
    c = 2;//正确
    cout << "c=" << c << ",d=" << d << endl;//c=2,d=2
}


float temp;//定义全局变量temp
float fn1(float r);//声明函数fn1
float &fn2(float r);//声明函数fn2 r
float fn1(float r) {//定义函数fn1，它以返回值的方法返回函数值
    temp = (float) (r * r * 3.14);
    return temp;
}

float &fn2(float r) {//定义函数fn2，它以引用方式返回函数值
    temp = (float) (r * r * 3.14);
    return temp; // 全局变量，不能返回局部变量的引用
}

void fun3() {
    float a = fn1(1);//第1种情况，系统生成要返回值的副本（即临时变量）
//    float &b=fn1(10.0); //第2种情况，可能会出错（不同 C++系统有不同规定）
/*
    编译错误：cannot convert from 'float' to 'float &'
    A reference that is not to 'const' cannot be bound to a non-lvalue
*/
    //不能从被调函数中返回一个临时变量或局部变量的引用
    float c = fn2(2);//第3种情况，系统不生成返回值的副本 (c=temp)
    cout << "a=" << a << ",c=" << c << endl; // a=3.14,c=12.56
    //可以从被调函数中返回一个全局变量的引用
    float &d = fn2(3); //第4种情况，系统不生成返回值的副本  (&d=temp)
    cout << "a=" << a << ",c=" << c << ",d=" << d << endl; //a=3.14,c=12.56,d=28.26
    float &e = fn2(4); //第4种情况，系统不生成返回值的副本 (d也会跟着变化)
    cout << "a=" << a << ",c=" << c << ",d=" << d << ",e=" << e << endl; //a=3.14,c=12.56,d=50.24,e=50.24
    //a=314,c=314,d=314
}

int main() {
    fun1();
    fun2();
    fun3();
    return 0;
}

// 引用是除指针外另一个可以产生多态效果的手段。这意味着，一个基类的引用可以指向它的派生类实例。

