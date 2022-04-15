#include "10-5.h"
#include <iostream>
using namespace std;

Stack::Stack()
{
   top = 0;
   sum = 0;
   cout<<"start top"<<top<<"start sum"<<sum<<endl;
}

bool Stack::isempty()const
{
	return top == 0;//可以不加括号
}

bool Stack::isfull()const
{
    return top == MAX;
}

void Stack::push(const Item& it)//括号里 1.const没加 2.Item& 写不出
{
    if(!isfull())//不是MAX-1
    {
        //下面2句顺序错，然后 items[MAX+1]非法空间，然后top跟着错了（至少表现出是这样，没到items[top] = it;这行时还是MAX，到了就变了
        items[top] = it;
        top++;
        cout<<"++:"<<top<<endl;
    }
    else
    {
        cout << "Space is not enough" << endl;
    }
}

void Stack::pop(Item& it)//引用确实需要初始化。但这边的知识点是：引用做函数传参.不能加const,进了函数变了
{
    if(!isempty())//不是MAX-1
    {
        it = items[top];
        sum += it.payment;
        top--;
        cout<<"--:"<<top<<endl;
    }
    else
    {
        cout << "no item" << endl;
    }

    
}

void Stack::show()
{
   cout << "top:" << top << "sum" << sum <<endl;

   for(int i=0;i<top;i++)
   {
       cout << items[i].fullname << ":" << items[i].payment << endl;
   }
}