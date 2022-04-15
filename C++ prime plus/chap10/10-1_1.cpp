//类声明 VS 方法实现 ： 专有名词不熟  见https://blog.csdn.net/tianguiyuyu/article/details/102978723示例

#include <iostream>

using namespace std;

class account{
    public:
      //account(string name,string banknumber,int despoit):m_name(name),m_banknumber(banknumber),m_despoit(despoit)不能用，因为成员private,只有成员是public才能用
      account(string name,string banknumber,int despoit)
      {
          m_name = name;
          m_banknumber = banknumber;
          m_despoit = despoit;
      }

      void show()
      {
          cout<< "name:"<< m_name << ",";
          cout<< "banknumber:"<< m_banknumber << ",";
          cout<< "despoit:"<< m_despoit << ",";
          cout<< endl;
      }

      void withdraw(int despoit)
      {
         this->m_despoit += despoit;
      }

      void save(int despoit)
      {
         this->m_despoit -= despoit;
      }

    private:
      string m_name;
      string m_banknumber;
      int m_despoit;
};//;容易漏
