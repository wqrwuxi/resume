#include "10-4.h"
#include<numeric> //accumulate
#include <iomanip>
using namespace SALES;


Sales::Sales(double array[],int num)
{
   vector<double> arr;

    for(int i=0;i<num;i++)
    {
        arr.push_back(array[i]);
        sales[i] = array[i];//漏
    }

    for(int i=num;i<QUARTERS;i++)
    {
        sales[i] = 0;
    }

    average = accumulate(arr.begin(), arr.end(), 0.0) / arr.size();
    max = *max_element(arr.begin(), arr.end());
    min = *min_element(arr.begin(), arr.end());

}

Sales::Sales()
{
    double array[4];
    cout << "Enter 4 double number:" << endl;

    for(int i=0;i<4;i++)
    {
        //cin >> array[i];
        scanf("%lf",&array[i]);
    }

    //Sales(array,4);不能构造函数套构造函数！！！！！！！！！！！！！！新建的会覆盖之前的class的元素
    vector<double> arr;

    for(int i=0;i<QUARTERS;i++)
    {
        arr.push_back(array[i]);
        sales[i] = array[i];//漏
    }

    average = accumulate(arr.begin(), arr.end(), 0.0) / arr.size();
    max = *max_element(arr.begin(), arr.end());
    min = *min_element(arr.begin(), arr.end());

}

void Sales::show()
{
    cout << "4 double number:";
    for(int i=0;i<4;i++)
    {
        //cout << setprecision (2) << sales[i] << " "; 理解有误
        printf("%.2lf ",sales[i]);
        //cout << sales[i] << " ";
    }
    cout << endl;

    cout << "average:" << average << endl;
    cout << "max:" << max << endl;
    cout << "min:" << min << endl;
}

int main()
{
    double ar[3] = { 32.1, 23.2, 65.3 };
    Sales sales1(ar,3);
    Sales sales2;
    
    sales1.show();
    sales2.show();
    

}

