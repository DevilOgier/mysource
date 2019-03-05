#include <cstdlib>
#include <cstring>

// 更详细的实现见： https://blog.csdn.net/zhengqijun_/article/details/55106090

class String {
public:
    String(); //默认构造函数
    String(const char *data); //普通构造函数
    String(const String &other);// 拷贝构造函数

    // 声明和定义操作符时，把操作符看错函数名就好
    String &operator=(const String &other);// 赋值函数

    ~String();// 析构函数

private:
    char *m_data;
};


String::String() {
    m_data = new char[1];
    m_data[0] = '\0';
}

String::String(const String &other) {
    m_data = new char[strlen(other.m_data) + 1];
    memcpy(m_data, other.m_data, strlen(other.m_data));
    m_data[strlen(other.m_data)] = '\0';
}

String::String(const char *data) {
    if (data == NULL) {
        m_data = new char[1];
        m_data[0] = '\0';
    }
    m_data = new char[strlen(data) + 1];
    memcpy(m_data, data, strlen(data));
    m_data[strlen(data)] = '\0';
}

String &String::operator=(const String &other) {
    if (this == &other) { //得分点：检查自赋值
        return *this;
    }
    if (m_data != NULL) {
        delete[]m_data; //得分点：释放原有的内存资源
    }
    m_data = new char[strlen(other.m_data) + 1];
    memcpy(m_data, other.m_data, strlen(other.m_data));
    m_data[strlen(other.m_data)] = '\0';
    return *this;

}

String::~String() {
    delete[]m_data;
    m_data = NULL;
}


int main() {
    String s("1234");
}











// g++ -std=c++11 string_class.cpp -o /home/test && /home/test