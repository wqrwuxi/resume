#include "10-6.h"
#include <iostream>
using namespace std;

int main()
{
    Move de;
    cout << "de" <<endl;
    de.showmove();
    
    Move for_add(2,3);
    Move origin(1,2);
    Move new_one = origin.add(for_add);
    cout << "new_one" <<endl;
    new_one.showmove();
    origin = for_add + origin;
    cout << "origin" <<endl;
    origin.showmove();


    origin.reset();
    cout << "origin reset" <<endl;
    origin.showmove(); 

}