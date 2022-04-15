//#include "10-1.h"报错：10-1-main.cpp:(.text+0x94): undefined reference to `account::show()'，10-1-main.cpp:(.text+0xa8): undefined reference to `account::save(int)'等等
#include "10-1.cpp"

int main()
{
    string name = "A";//string不熟练，string name只做了个声明，没说里面的内容叫name
    string maccount = "123456A";
    int init = 0;
    
    //结论，无论 .h文件加不加account() {}； account my("name","maccount",0); 和 account my(name,maccount,init);都可以
    account my("name","maccount",0);
    account my(name,maccount,init);

    //.h文件不加account() {};时
    //不能有这个：account my;这个之前一只理解有误，其实相当于调用了一个无参构造函数  error: no matching function for call to ‘account::account()’
    //只能这个：account my(name,maccount,init);//因为自己写了有参构造函数，就无默认无参构造函数，声明只能按有参构造函数的方法声明
    //account my("name","maccount",0);//也可以
    
    //h文件加account() {};时  account my；就可以用了
    //以下2行说明有参构造函数的传入值必须是 直接值，不能用某种类型的变量名，会变成传的地址。而且string/char*不好处理
    //account my;my(name,maccount,init);//报错 error: no match for call to ‘(account) (std::__cxx11::string&, std::__cxx11::string&, int&)’因为没定义仿函数
    //account my;my("name","maccount",0);//error: no match for call to ‘(account) (const char [5], const char [9], int)’  因为没定义仿函数
    //account my("name","maccount",0);可以

    //account my;my(string("name"),string("maccount"),0);//error: no match for call to ‘(account) (std::__cxx11::string, std::__cxx11::string, int)’
    //上面那行，那个有参构造函数，构造函数干什么用的？创建对象时为对象的成员属性赋值，且只会调用一次。上面那个my什么什么的，那根本不是用构造函数了
    //Q：那error指的是：函数调用运算符重载（） 即 仿函数，因为类似于函数调用

    my.show();

    my.save(10);

    my.show();

    my.withdraw(5);

    my.show();
}