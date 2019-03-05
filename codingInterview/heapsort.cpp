#include <iostream>       //std::cout
#include <cstdlib>
#include <vector>
#include <algorithm>

#define Len 10

// https://www.cnblogs.com/chengxiao/p/6129630.html  图解堆排序

//start 为原根节点，end 节点数，参与调整的为[start,end]中为start子孙的节点，调整时是已知两个子树都是堆
void heapAdjust(int R[], int start, int end)
{
    int j, temp;
    temp = R[start];  //辅助量，保存原根结点
    //由于根节点是从0开始，所以j的左孩子为 2*j+1，右孩子为2*j+2
    for ( j = 2 * start + 1; j <= end; j = j * 2 + 1 ) {

        if ( j < end && R[j] < R[j + 1] ) {
            ++j;  //右孩子比较大 j指向R[i]的右孩子
        }
        if ( temp >  R[j] ) {   // 根＞孩子，已是堆，调整结束
            break;
        }
        R[start] = R[j];
        start = j;
    }
    R[start] = temp;  //原根结点放入正确位置
}

/**
  * @brief 堆排序
  * @param R为待排序的数组，size为数组的长度
  *  时间复杂度:构建大(小)顶堆，完全二叉树的高度为log(n+1)，因此对每个结点调整的时间复杂度为O(logn)
  *           两个循环，第一个循环做的操作次数为n/2，第二个操作次数为(n-1)，因此时间复杂度为O(nlogn)
  */
void heapSort(int R[], int size)
{
    int i, temp;
    for ( i = size / 2 - 1; i >= 0; --i ) {  //前size/2个为非叶子节点，size / 2 - 1为第一个非叶子结点
        heapAdjust(R, i, size -1);  //建初始堆
    }
    for ( i = size - 1; i >=1; --i ) { //进行n-1趟堆排序,堆重建后,R[0]最大，放到最后，重新建堆
        temp = R[i];
        R[i] = R[0];
        R[0] = temp;//表尾和表首的元素交换
        heapAdjust(R, 0, i - 1);//把表首的元素换成表尾的元素后，重新构成大顶堆，因为除表首的元素外，
        //后面的结点都满足大顶堆的条件，故heapAdjust()的第二个参数只需为0
    }
}

void print(int *data, int len) {
    for (int i = 0; i < len; i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int data[Len] = {25, 3, 5, 2, 6, 8, 1, 9, 10, 7};
    heapSort(data,Len);
    print(data, Len);
}

// g++ -std=c++11 heapsort.cpp -o /home/test && /home/test