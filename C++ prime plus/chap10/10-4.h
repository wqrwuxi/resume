#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

namespace SALES
{
    const int QUARTERS = 4;
    class Sales{
      public:
        Sales(double array[],int num);
        Sales();
        void show();//void 不能少

      private:
        double sales[QUARTERS];
        double average;
        double max;
        double min;
    };
}