#include "10-5.h"
//using namespace std;  to_string在命名空间std中，要加
#include <cstdio>

int main()
{
    Item now;
    Stack s;
    unsigned i;

    for(i=0;i<5;i++)
    {
       sprintf(now.fullname, "%d", i);
       now.payment = i;
       s.push(now);
    }
    
    s.show();

    for(i=0;i<5;i++)
    {
       s.pop(now);
    }
}