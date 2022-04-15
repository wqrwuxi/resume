#include "10-1.h"
//这种写法，构造函数还是构造函数的样子，普通成员函数就是普通函数的样子
account::account(string name,string banknumber,int despoit)
{
    m_name = name;
    m_banknumber = banknumber;
    m_despoit = despoit;
}

void account::show()
{
    cout<< "name:"<< m_name << ",";
    cout<< "banknumber:"<< m_banknumber << ",";
    cout<< "despoit:"<< m_despoit << ",";
    cout<< endl;
}

void account::save(int despoit)
{
    this->m_despoit += despoit;
}

void account::withdraw(int despoit)
{
    this->m_despoit -= despoit;
}


