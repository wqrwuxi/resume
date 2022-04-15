#include "stonewt.h"

int main()
{
    Stonewt one(10,8);
    Stonewt two;

    one.show();
    two.show();
    //(one * 4.6).show();
    (one * 4.6).show();
    (13 + one).show();
    return 0;
}