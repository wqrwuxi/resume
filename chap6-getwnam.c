#include <pwd.h>
#include <string.h>//提供NULL原型
#include <stdio.h>//提供printf原型
#include <stdlib.h>//提供malloc原型
#include <stddef.h>//没找到这个提供什么

struct passwd* getwnam(const char* name)//加const限定符比较好
{
    setpwent();//保护措施，书P151

    //下面两行gcc -o 1-1 1-englishToMorse.cN都可以
    //struct passwd *ptr=malloc(sizeof(struct passwd));
    struct passwd *ptr;

    while((ptr=getpwent())!=NULL)
    {
        if ((strcmp(ptr->pw_name,name))==0)
        {
           break;
        }
    }
    endpwent();
    return (ptr); //ptr是指针，*ptr就解引用了 又不是定义！
}

void main(void)
{
    struct passwd *p=getwnam("wqrr");//若不存在：bash: ./6-1: No such file or directory
    printf("pw_name:%s,pw_passwd:%s,pw_uid:%d,pw_gid:%d\n",p->pw_name,p->pw_passwd,p->pw_uid,p->pw_gid);
    printf("pw_gecos:%s,pw_dir:%s,pw_shell:%s\n",p->pw_gecos,p->pw_dir,p->pw_shell);

}

/*https://www.mkssoftware.com/docs/man5/struct_passwd.5.asp#:~:text=struct%20passwd%20data%20structure%20containing%20user%20account%20information,ALSO.%20PTC%20MKS%20Toolkit%2010.3%20Documentation%20Build%2039.
#include <pwd.h>

struct passwd {
	char *pw_name;
	char *pw_passwd;
	uid_t pw_uid;
	gid_t pw_gid;
	time_t pw_change;
	char *pw_class;
	char *pw_gecos;
	char *pw_dir;
	char *pw_shell;
	time_t pw_expire;
};

*/