#include "10-8.h"

int main()
{
    List llist1;
    llist1.addData("name1",1);
    llist1.addData("name2",2);
    llist1.addData("name3",3);
    llist1.addData("name4",4);
    llist1.addData("name5",5);
    llist1.addData("name6",6);
    llist1.visit(show);

}