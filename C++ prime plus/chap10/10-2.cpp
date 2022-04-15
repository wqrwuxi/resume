#include "10-2.h"
#include <cstring>

Person::Person(const string &ln,const char* fn)//const char* fn="Heyyou"的"Heyyou"不能写
{
    this->lname = ln;
    //this->fname = fn;
    strncpy(fname, fn, LIMIT);//上一行会报： error: incompatible types in assignment of ‘const char*’ to ‘char [25]’
}

void Person::FormalShow() const//void漏
{
    cout << this->fname << " " << this->lname <<endl;
}

void Person::Show() const//void漏
{
    cout << this->lname << " " << this->fname <<endl;
}

int main()
{
    Person one;
    Person two("Smy");
    Person three("Dim","Sam");
    one.Show();
    one.FormalShow();
    two.Show();
    two.FormalShow();
    three.Show();
    three.FormalShow();
}