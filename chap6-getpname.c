#include <pwd.h>
#include <string.h>//提供NULL原型
#include <stdio.h>//提供printf原型
#include<shadow.h>
//6.3阴影口令   一直说文件不存在
struct spwd* getwnam()//加const限定符比较好
{
    setspent();//保护措施，书P151

    //下面两行gcc -o 1-1 1-englishToMorse.cN都可以
    //struct passwd *ptr=malloc(sizeof(struct passwd));
    struct spwd *ptr;

    /*
    while((ptr=getspent())!=NULL)
    {
        if ((strcmp(ptr->sp_namp,name))==0)
        {
           break;
        }
    }*/
    ptr=getspnam("root");
    endspent();
    return (ptr); //ptr是指针，*ptr就解引用了 又不是定义！
}
void main(void)
{
    struct spwd *p=getwnam();
    printf("用户登陆名:%s,加密口令:%s\n",p->sp_namp,p->sp_pwdp);
    printf("要求更改尚余天数:%d,经过多少天允许更改:%d,超期警告天数:%d\n",p->sp_max,p->sp_min,p->sp_warn);
    printf("账户超期天数:%d,保留:%d，账户不活动尚余天数:%d\n",p->sp_expire,p->sp_flag,p->sp_inact);
    printf("上次更改口令经过的天数:%d\n",p->sp_lstchg);

}