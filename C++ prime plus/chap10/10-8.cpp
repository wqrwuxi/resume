#include "10-8.h"

List::List()
{
    items = 0;
}

bool List::isempty()
{
   if(items==0)
   {
       return true;
   }

   return false;
}

bool List::isfull()
{
   if(items==LIST_SIZE)
   {
       return true;
   }

   return false;
}

bool List::addData(const string &name,int age)
{
    if(List::isfull()==false)
    {
        list_item[items].name = name;
        list_item[items].age = age;
        items++;
        return true;
    }

    return false;
}

bool List::delectData()
{
    if(List::isempty()==false)
    {
        items--;
        list_item[items].name = "";
        list_item[items].age = 0;
        return true;
    }

    return false;
}

// void List::show(Item &item)
// {
//     cout << "name:" << item.name << "  age:" << item.age;
// }

void show(Item& item)
{
    
        cout << "name:" << item.name << "  age:" << item.age;
}

// void List::double_age(Item &item)
// {
//     item.age = item.age * 2;
//     cout << "name:" << item.name << "  age:" << item.age;
// }

void List::visit(void(*pf)(Item&))
{
    for(int i=0;i<items;i++)
    {
        pf(list_item[i]);
    }
}

