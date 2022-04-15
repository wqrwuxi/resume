#include "10-7.h"
#include <iostream>
#include <cstring>
using namespace std;

plorg::plorg(const char* new_name,int new_CI)
{
    strncpy(name, new_name, 20);
    IC = new_CI;
}

plorg::plorg()
{
    strncpy(name, "Plorg", 20);
    IC = 0;
}

void plorg::change_plog(int num)
{
    IC = num;
}

void plorg::show() const
{
    cout << "name" << name << "IC" << IC << endl;
}

