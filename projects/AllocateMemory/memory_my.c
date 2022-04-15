/**
 * @file memory.c
 * @author 智识之道 (binary@techbootcamp)
 * @brief 连续内存分配
 * @version 0.1
 * @date 2022-01-05
 *
 * @copyright Copyright (c) 2022
 *
 */

///////////////////////////////////////////////////////////////////////////////
//头文件区域
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
//有需要增加其他头文件在这里添加

///////////////////////////////////////////////////////////////////////////////
//宏定义区域

#define MEMSIZE 80
#define LINELEN 80
#define INSTRUCTNUM 20
#define WORD_MAX_NUM 3
#define WORD_MAX_LEN 20
//有需要增加其他头宏定义在这里添加

///////////////////////////////////////////////////////////////////////////////
//结构体区域

/**
 * @brief 内存首部结构体
 *
 */
typedef struct
{
    //下一个     这个void*，自己写不出
    void *next;
    //数据起始地址
    void *pack;
    int size;//当前使用大小
    int space;//整个空白的大小
    //是否正在使用
    int used;
    char name;
} memory_header;//感觉叫Node更好

typedef struct
{
    memory_header* node;
    void* next;
} sort_node;//感觉叫Node更好

/**
 * @brief 命令结构体
 *
 */
typedef struct
{
    //命令
    char *command;
    //命令函数
    int (*command_func)();//去看C++ primer plus上的函数指针
} command_info;

///////////////////////////////////////////////////////////////////////////////
//函数声明区域   放在这是为command_info commands[]服务的，而且没有参数，就不会出现没declare的情况
/**
 * @brief 调试：打印当前内存分配的详细情况，方便调试。
 *
 * @return int
 */
int mem_dump();

/**
 * @brief 分配内存：使用上述 3 种分配算法之一为进程分配 N 个字节。
 *
 * @return int
 */
int mem_allocate();
void *mem_alloc(char name, int size, char algo);

/**
 * @brief 释放内存：释放给定进程的所有分配。
 *
 * @return int
 */
int mem_free();

/**
 * @brief 显示内存：显示内存池的状态(已分配和空闲块)。
 *
 * @return int
 */
int mem_show();

/**
 * @brief 读取脚本：读取脚本，执行文件中的每个命令。
 *
 * @return int
 */
int mem_read();

/**
 * @brief 紧凑内存：紧凑分配，成为一个连续的块。
 *
 * @return int
 */
int mem_compact();

/**
 * @brief 退出程序：退出当前的程序。
 *
 * @return int
 */
int mem_exit();

//有需要增加其他函数声明在这里添加
int select_cmd();
///////////////////////////////////////////////////////////////////////////////
//变量区域

//内存首部地址
memory_header *first_header;
sort_node *best_fit_header;

//是否继续运行
int should_run = 1;

command_info commands[] = {
    {"D", &mem_dump},
    {"A", &mem_allocate},
    //{"A", &mem_alloc},
    {"F", &mem_free},
    {"S", &mem_show},
    {"R", &mem_read},
    {"C", &mem_compact},
    {"E", &mem_exit}};

//有需要增加其他变量在这里添加
char message[LINELEN];
///////////////////////////////////////////////////////////////////////////////
//函数区域

//有需要增加其他函数在这里添加
void handler();
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief 内存池初始化
 *
 * @return int 正确返回0,错误返回负数
 */
int mem_init()
{
    first_header = malloc(sizeof(memory_header));
    first_header->next = NULL;
    first_header->pack = malloc(MEMSIZE);
    first_header->size = MEMSIZE;
    first_header->space = MEMSIZE;
    first_header->used = 0;
    first_header->name = '.';
    return((first_header->pack==NULL)?-1:0);
}

/**
 * @brief First-Fit
 *
 * @param name 名字
 * @param size 大小
 * @return void* 分配的首地址
 */
void *mem_alloc_for_first_fit(char name, int size)
{
    memory_header* p = first_header;
    memory_header* q = first_header;

    while(p)
    {
        if((p->used==0) && (p->space >= size) && (p->next!=NULL))//该情况为释放后内存的再次分配情况
        {
            memory_header* Node = malloc(sizeof(memory_header));
            Node->name = name;
            Node->next = p;
            Node->pack = p->pack;
            Node->size = size;
            Node->space = size;
            Node->used = 1;
            
            p->pack = Node->pack + Node->size;
            p->space = p->space - size;

            if(p==first_header)
            {
                first_header=Node;
            }
            else
            {
                q->next = Node;
            }

            if(p->space==0)
            {
                Node->next = p->next;
                free(p);
                p = NULL;
                
            }

            return p->pack;
        }

        q = p;
        p = p->next;
    }
    
    //是的，无论什么点都叫Node，相当于没有名字，确定它是谁全靠上面几行的寻找
    //该情况为80个单位没有全分配完时的情况
    if(q->size >= size)
    {
        memory_header* Node = malloc(sizeof(memory_header));
        Node->next = NULL;
        Node->pack = q->pack + size;
        Node->size = q->size - size;
        Node->used = 0;
        Node->name = '.';
        Node->space = q->space - size;
        q->next = Node;
        q->size = size;
        q->used = 1;
        q->name = name;
        q->space = size;

        if(Node->space==0)
        {
            q->next = NULL;
            free(Node);
            Node = NULL;
            
        }
        return q->pack;
    }
    else
    {
        printf("Space is not enough.\n");
        return NULL;
    }

    
}

/**
 * @brief Best-Fit
 *
 * @param name 名字
 * @param size 大小
 * @return void* 分配的首地址
 */
void *mem_alloc_for_best_fit(char name, int size)
{ 
    memory_header* p = first_header;
    memory_header* q = first_header;
    memory_header* min = NULL;
    memory_header* min_forward = NULL;
    int min_space = MEMSIZE;

    if(first_header->used==0)
    {
        min = first_header;
        min_space = min->space;
    }
    else
    {
        do{
           p = q->next;

           if((p->used==0) && (p->space >= size) && (p->space < min_space) )
           {
              min_forward = q;
              min = p;
              min_space = min->space;
           }
        
           q = q->next;
        }while(q->next);
    }
   
    if(min==NULL)
    {
        printf("Space is not enough.\n");
        return NULL;
    }
    else if(min->next!=NULL)
    {
       
        memory_header* Node = malloc(sizeof(memory_header));
        Node->name = name;
        Node->next = min;
        Node->pack = min->pack;
        Node->size = size;
        Node->space = size;
        Node->used = 1;
        
        min->pack = Node->pack + Node->size;
        min->space = min->space - size;

        if(min==first_header)
        {
            first_header=Node;
        }
        else
        {
            min_forward->next = Node;
        }

        if(min->space==0)
        {
            Node->next = min->next;
            //free只是扔回内存池，那个东西依旧指向原来的
            free(min);//少复制粘贴！错了查也查不出！
            min = NULL;
            
        }
    }
    else
    {
        memory_header* Node = malloc(sizeof(memory_header));
        Node->next = NULL;
        Node->pack = min->pack + size;
        Node->size = min->size - size;
        Node->used = 0;
        Node->name = '.';
        Node->space = min->space - size;
        min->next = Node;
        min->size = size;
        min->used = 1;
        min->name = name;
        min->space = size;

        if(Node->space==0)
        {
            min->next = NULL;
            free(Node);
            Node = NULL;
            
        }
    }
}

/**
 * @brief Worst-Fit
 *
 * @param name 名字
 * @param size 大小
 * @return void* 分配的首地址
 */
void *mem_alloc_for_worst_fit(char name, int size)
{
    memory_header* p = first_header;
    memory_header* q = first_header;
    memory_header* max = NULL;
    memory_header* max_forward = NULL;
    int max_space = -1;

    if(first_header->used==0)
    {
        max = first_header;
        max_space = max->space;
    }
    else
    {
        do{
           p = q->next;

           if((p->used==0) && (p->space >= size) && (p->space > max_space) )
           {
              max_forward = q;
              max = p;
              max_space = max->space;
           }
        
           q = q->next;
        }while(q->next);
    }
    
    if(max==NULL)
    {
        printf("Space is not enough.\n");
        return NULL;
    }
    else if(max->next!=NULL)
    {
        if(max->space==size)
        {
            max->name = name;
            max->size = size;
            max->used = 1;
        }
        else
        {
            memory_header* Node = malloc(sizeof(memory_header));
            Node->name = name;
            Node->next = max;
            Node->pack = max->pack;
            Node->size = size;
            Node->space = size;
            Node->used = 1;
        
            max->pack = Node->pack + Node->size;
            max->space = max->space - size;

            if(max==first_header)
            {
                first_header=Node;
            }
            else
            {
                max_forward->next = Node;
            }
        }
    }
    else
    {
        if(max->space==size)
        {
            max->name = name;
            max->size = size;
            max->used = 1;
        }
        else
        {
            memory_header* Node = malloc(sizeof(memory_header));
            Node->next = NULL;
            Node->pack = max->pack + size;
            Node->size = max->size - size;
            Node->used = 0;
            Node->name = '.';
            Node->space = max->space - size;
            max->next = Node;
            max->size = size;
            max->used = 1;
            max->name = name;
            max->space = size;
        }
        
    }
}

/**
 * @brief 根据指定的算法分配内存
 * 注：此函数无须增加内容。
 *
 * @param name 名字
 * @param size 大小
 * @param algo 指定算法
 * @return void* 分配的首地址
 */
void *mem_alloc(char name, int size, char algo)
{
    switch (algo)
    {
    case 'F':
        mem_alloc_for_first_fit(name, size);
        break;

    case 'B':
        mem_alloc_for_best_fit(name, size);
        break;

    case 'W':
        mem_alloc_for_worst_fit(name, size);
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////

int mem_dump()
{
    memory_header *p = first_header;
    memory_header *q = first_header;
    int count = 1;

    printf("---------------------------------------------------------------------------------\n");
    printf("****************************MemoryHeader****************************************\n");
    printf("id        *         next         pack         size        used   name  space\n");
    
    do{
       printf("%-10d%-10x%-13x%-13x%-12d%-7d%-6c%d\n",count,q,q->next,q->pack,q->size,q->used,q->name,q->space);
       count++;
       q = q->next;
    }while(q);
    
    printf("****************************MemoryHeader****************************************\n");
    return 1;
}

int mem_allocate()
{
    int str_len = strlen(message);
    char num[str_len];
    int count = 0;
    
    memset(num,' ',str_len);

    for(int i=4;i<str_len;i++)//因为不是从0开始，不顺手于是后面两个参数倒了倒
    {
       if(message[i]!=' ')
       {
           num[i-4] = message[i];
       }
       else
       {
           count = i+1;
           break;
       }
    }

    mem_alloc(message[2], atoi(num), message[count]);
    return 1;
}

int mem_free()
{
    memory_header *p = first_header;

    do{
       if(message[2]==p->name)
       {
           p->name = '.';
           p->size = 0;
           p->used = 0;
       }

       p = p->next;
    }while(p);

    p = first_header;
    memory_header *q = first_header;
   
    do{
        q = p->next;

        if((p->used==0) && (q->used==0))
        {
            p->space = p->space + q->space;
            p->next = q->next;
            free(q);
            q = NULL;
        }
        else
        {
            p = p->next;
        }
    
    }while(p->next);

    return 1;
}

int mem_show()
{
    memory_header *p = first_header;
    
    do{
       int num1 = p->size;
       int num2 = p->space - p->size;

       for(int i=0;i<num1;i++)
       {
          printf("%c",p->name);
       }

       for(int i=0;i<num2;i++)
       {
           printf("%c",'.');
       }

       p = p->next;
    } while(p);
    
    return 1;
}

int mem_read()//功能：新进程信息输入方式：R指令（读脚本）  &&  stdin
{
    char file[WORD_MAX_LEN] = {0};//指定了第一个字符，剩余的全部用'\0'填充
    int file_len = strlen(message)-2;
    for(int i=0;i<file_len-1;i++)//最后一个回车去除
    {
        file[i] = message[i+2];
    }
    
    FILE* fp = fopen(file,"r");
    if(fp==NULL)
    {
        printf("fopen error NO.%d:%s\n",errno,strerror(errno));
    }
    
    while(fgets(message,LINELEN,fp)!=NULL)
    {
        printf("%s\n",message);
        handler();
    }
        
    return 1;
}

int mem_compact()
{
    memory_header *p = first_header;
    memory_header *q = NULL;
    memory_header *temp;
    int used_count = 0;
    
    do{
        if(p->used==1)
        {
            used_count = used_count + p->size;
            q = p;
            q->pack = first_header->pack;
            first_header = q; 
            break;
        }
        
        p = p->next;
    }while(p);
    
    if(p!=NULL)
    {
        while(p->next)
        {
           temp = p->next;
           if(temp->used==1)
           {
               used_count = used_count + temp->size;
               q->next = temp;
               temp->pack = q->pack + q->size;
               q = temp;
           }

           p = temp;  
        }
    }

    memory_header* end = malloc(sizeof(memory_header));
    q->next = end;
    end->name = '.';
    end->next = NULL;
    end->pack = q->pack + q->size;
    end->size = MEMSIZE - used_count;
    end->space = MEMSIZE - used_count;
    end->used = 0;
    
    return 1;
}

int mem_exit()
{
    exit(0);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * @brief 命令个数
 *
 * @return int
 */
int cmd_num()
{
    printf("TODO: cmd_num");
    return 0;
}

/**
 * @brief 显示标记
 *
 */
void show_mem()
{
    printf("mem>");
}

/**
 * @brief 显示用法
 *
 */
void show_usage()
{
    printf("Usage:\n");
    printf("D\n");
    printf("A <name> <size> <algo>\n");
    printf("F <name>\n");
    printf("S\n");
    printf("R <file>\n");
    printf("C\n");
    printf("E\n");
    printf("Type program names and arguments, and hit enter.\n");
}

/**
 * @brief 解析命令
 *
 * @param fp
 * @return int
 */

int parse_cmd(FILE* fp)
{
    if(fgets(message,LINELEN,fp)==NULL)
    {
        printf("fgets error NO.%d:%s",errno,strerror(errno));
        return 0;
    }

    return 1;
}

/**
 * @brief 选择命令
 *
 * @return int 未找到返回-1,找到了返回数组下标
 */
int select_cmd()
{
    if(message[0]=='D')
    {
        return 0;
    }
    else if(message[0]=='A')
    {
        return 1; 
            
    }
    else if(message[0]=='F')
    {
        return 2;    
    }
    else if(message[0]=='S')
    {
        return 3; 
    }
    else if(message[0]=='R')
    {
        return 4;
    }
    else if(message[0]=='C')
    {
        return 5;
    }
    else if(message[0]=='E')
    {
        return 6;
    }
    else
    {
        return -1;
    }
}

/**
 * @brief 重置数据（如有必要）
 *
 */
void reset()
{
    printf("TODO: reset");
}

/**
 * @brief 处理命令
 * 注：此函数无须增加内容。
 *
 */
void handler()
{
    int pos = select_cmd();

    if (pos >= 0)
    {
        should_run = commands[pos].command_func();
    }

    //reset();
}
////////////////////////////////////////////

/**
 * @brief 主函数入口
 * 注：此函数无须增加内容。
 *
 * @param argc 参数个数
 * @param argv 参数数组
 * @return int 返回值
 */
int main(int argc, char *argv[])
{
    if (mem_init())
    {
        exit(1);
    }

    show_usage();

    while (should_run)
    {
        show_mem();

        if (!parse_cmd(stdin))
        {
            continue;
        }

        handler();
    }

    return 0;
}
