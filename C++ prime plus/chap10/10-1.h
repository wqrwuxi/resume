
#pragma once
#include <iostream>

using namespace std;

class account{
    public:
      account() {};
      account(string name,string banknumber,int despoit);
      void show();
      void withdraw(int despoit);
      void save(int despoit);
      
    private:
      string m_name;
      string m_banknumber;
      int m_despoit;
};//;容易漏