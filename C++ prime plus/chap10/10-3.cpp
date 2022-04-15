#include <iostream>
#include <string.h>
using namespace std;
//难点：1.class下如何创建数组  2.涉及char数组的构造函数及后期赋值如何处理（L16+L20） 3.有参构造函数的调用（L38）/拷贝构造的调用（L36）4.cin使用不熟
const int LEN = 40;

class golf
{
   public:
     //如果少，10-3.cpp:48:15: error: no matching function for call to ‘golf::golf()’  golf group[3];
     //golf(){fullname[0]='\0';handicap=0;} 在声明char* fullname时不行，都不知道多大，怎么[0]
     golf(){};
     //0-3.cpp:10:23: warning: ISO C++ forbids converting a string constant to ‘char*’ [-Wwrite-strings] golf(char* name= "\0",int hc = 0)
     //10-3.cpp:12:21: error: invalid conversion from ‘const char*’ to ‘char*’ [-fpermissive] fullname = name;
     //  10-3.cpp:12:21: error: incompatible types in assignment of ‘const char*’ to ‘char [40]’   fullname = name;   
     golf(char* name,int hc)
     {
         //fullname = name;
         //10-3.cpp:18:21: error: incompatible types in assignment of ‘char*’ to ‘char [40]’ fullname = name;解决如下
         strncpy(fullname, name, LEN);
         handicap = hc;
     }

     int setgolf()
     {
         char* temp;
         int num;

         cout << "Please enter the full name of golf player: ";
         cin.getline(temp, LEN);
 
         cout << "Please enter the hanicap of golf player: ";
         cin >> num;
         cin.get();

        //golf gg(temp,num);*this = gg;  可以，是拷贝构造的形式
        //构造函数的调用 Person p2 = Person(10)，解释了如下的存在
         *this = golf(temp, num);

         if(strcmp(this->fullname,"\0")==0)
         {
           return 0;
         }
         else
         {
           return 1;
         }
     }

     void print()
     {
       cout<<fullname<<" "<<handicap<<endl;
     }

   private:
     char fullname[LEN];
     int handicap;
};

int main()
{
  //golf* group[3];可以golf* group;group=(golf*)malloc 浑了
  //golf group[3]=(golf*)malloc(sizeof(golf)*3); error: array must be initialized with a brace-enclosed initializer 。char a[10] = {'0'}；可以，一比较就说明原因了
  //golf group[3]想要可以，golf(){fullname[0]='\0';handicap=0;}，即调用无参构造函数时，要指定值
  golf* group;
  group = (golf*)malloc(sizeof(golf)*3);
  int count=0;
  /*while(count<3 && group[count-1].setgolf()==1)
  {
      group[count].setgolf();
      count++;
  }*/
  int re = 0;
  do{
     re = group[count++].setgolf();
  }while(count<3 && re==1);

  for(int i=0;i<count;i++)
  {
    group[i].print();
  }
}
