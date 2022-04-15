#include <iostream>
using namespace std;

struct Item{string name;int age;};

class List{
  private:
    enum {LIST_SIZE = 5};
    
    Item list_item[LIST_SIZE];
    //int items;//current number of items in List
  public:
    List();
    bool addData(const string &name,int age);
    bool delectData();
    bool isempty();
    bool isfull();
    void visit(void(*pf)(Item&));
    int items;//current number of items in List
    //void show(Item&);
    //void double_age(Item &item);
    
};

void show(Item&);

// void show(Item &item);
//     void double_age(Item &item);