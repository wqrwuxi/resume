#pragma once
#define MAX 4 //头文件不能定义，会不会有问题

struct customer{
    char fullname[35];
    double payment;
};
typedef customer Item;

class Stack{
    
    public:
       Stack();
       bool isempty()const;//声明/实现的时候要加const，调用的时候不加const
	   bool isfull()const;
       void push(const Item& it);//括号里 1.const没加 2.Item& 写不出
       void pop(Item& it);//引用确实需要初始化。但这边的知识点是：引用做函数传参
       void show();

    private:
       Item items[MAX];
	   int top;
       double sum;
};