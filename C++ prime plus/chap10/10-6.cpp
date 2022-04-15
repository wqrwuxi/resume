#include "10-6.h"
#include <iostream>
using namespace std;
Move::Move(double a,double b)
{
    x = a;
    y = b;
}

void Move::showmove()const
{
    cout << "x:" << x << "y:" << y <<endl;
}

Move Move::add(const Move &m)const
{
    //Move* new_one = new Move(m);
    Move new_one;
    new_one.x = this->x + m.x;
    new_one.y = this->y + m.y;
    return new_one;
}

void Move::reset(double a,double b)
{
    x = a;
    y = b;
}

Move Move::operator+(const Move &m)const
{
    Move new_one;
    new_one.x = this->x + m.x;
    new_one.y = this->y + m.y;
    return new_one;
}
