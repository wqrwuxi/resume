/*注意几个类型转换
1.void*指针 = MemoryHeader*具体指针 + int
同时，等号右边说明：数+指针 可以赋值给指针
举例：void *get_data(MemoryHeader *current)
https://www.runoob.com/w3cnote/c-void-intro.html  ：void 指针可以指向任意类型的数据，就是说可以用任意类型的指针对 void 指针对 void 指针赋值
2.位运算操作 + 指针
由于指针是8个字节（也说明了系统是64位），只能将指针转为long类型和数字进行位运算
举例：int is_allocated(MemoryHeader *current)中  if(((long)current->packed & 1)==1)
3.long类型 转换类型成为指针
举例：MemoryHeader *get_next_header(MemoryHeader *current)中 MemoryHeader* next = (MemoryHeader*)((long)current->packed|1-1);
4.mem_dump()那几个参数的类型，是编译器要求这样的，注意p%和ld%
*/

/**
 * @file memory.h
 * @author 智识之道 (binary@techbootcamp)
 * @brief 内存分配
 * @version 0.1
 * @date 2022-01-18
 *
 * @copyright Copyright (c) 2022
 *
 */
///////////////////////////////////////////////////////////////////////////////
//头文件区域
//其他头文件自行添加

#include <stdio.h>
#include <stdlib.h>//malloc
#include "memory.h"

///////////////////////////////////////////////////////////////////////////////
//结构体区域

/**
 * @brief 内存结构体
 *
 */
typedef struct MemoryHeader
{
    //下一个首部地址+分配位
    void *packed;
    //分配的块大小或，或是可用有效负载大小
    unsigned size;
} MemoryHeader;

///////////////////////////////////////////////////////////////////////////////
//变量区域

//设定的算法
enum Algorithm algo;

//内存块头
MemoryHeader *memory_header;

int all_len;

///////////////////////////////////////////////////////////////////////////////
//函数声明区域

/**
 * @brief 是否运行分配
 *
 * @param current 当前内存块首部
 * @param size 需要分配块的大小
 * @return int 允许分配则返回1；不允许分配返回0
 */
int is_allow_allocate(MemoryHeader *current, int size);

/**
 * @brief 是否已经分配
 *
 * @param current 当前内存块首部
 * @return int 已经分配则返回1,未分配则返回0
 */
int is_allocated(MemoryHeader *current);

/**
 * @brief 是否空闲
 *
 * @param current 当前内存块首部
 * @return int 空闲则返回1,不空闲则返回0
 */
int is_free(MemoryHeader *current);

/**
 * @brief 获取下一个首部
 *
 * @param current 当前内存块首部
 * @return MemoryHeader* 有下一个则返回指针；如是最后一个则返回NULL
 */
MemoryHeader *get_next_header(MemoryHeader *current);

/**
 * @brief 获取分配的大小
 *
 * @param current 当前内存块首部
 * @return int 分配块的大小
 */
int get_size(MemoryHeader *current);

/**
 * @brief 获取数据的指针
 *
 * @param current 当前内存块首部
 * @return void* 写数据的指针
 */
void *get_data(MemoryHeader *current);

/**
 * @brief 从数据指针获取到首部指针
 *
 * @param current 当前内存块首部
 * @return MemoryHeader* 返回首部指针
 */
MemoryHeader *get_header_from_data(void *current);

/**
 * @brief 设置下一个首部地址+分配位
 *
 * @param current 当前内存块首部
 * @param size 分配的大小
 */
void set_next_packet(MemoryHeader *current, int size);

/**
 * @brief 设置已经分配的
 *
 * @param current 当前内存块首部
 */
void set_allocated(MemoryHeader *current);

/**
 * @brief 设置未分配的
 *
 * @param current 当前内存块首部
 */
void set_free(MemoryHeader *current);

/**
 * @brief 设置分配的大小
 *
 * @param current 当前内存块首部
 * @param size 分配块的大小
 */
void set_size(MemoryHeader *current, int size);

/**
 * @brief 合并
 *
 * @param current 当前内存块首部
 * @param last 上一个内存块首部
 */
void coalesce(MemoryHeader *current, MemoryHeader *last);

///////////////////////////////////////////////////////////////////////////////
//函数实现区域

int is_allow_allocate(MemoryHeader *current, int size)
{
    if(current->size >= size)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}

int is_allocated(MemoryHeader *current)
{
    if(((long)current->packed & 1)==1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}

int is_free(MemoryHeader *current)
{
    if(((long)current->packed & 1)==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

MemoryHeader *get_next_header(MemoryHeader *current)//需要非常清醒，current->packed是没法直接代表下一个地址的，一定要处理！
{
    MemoryHeader* next = (MemoryHeader*)(((long)current->packed|1)-1);//这括号很难弄对
    return next;
}

int get_size(MemoryHeader *current)
{
    return current->size;
}

void *get_data(MemoryHeader *current)
{
    void* data = (void*)current + sizeof(MemoryHeader);
    return data;
}

MemoryHeader *get_header_from_data(void *current)
{
    return (void*)current-sizeof(MemoryHeader);
}

void set_next_packet(MemoryHeader *current, int size)
{
    //size 是 next的size
    //根据current自身的数据就可以确定下一个数据的起始地址，即current的pack中的地址已经有了
    //next也是根据自身的数据就可以确定下一个数据的起始地址
    //这里和上一个项目的最大区别在于：上一次项目malloc了2个东西，一个是长长内存，还有就是每次新数据出现时malloc的那个node，这个node，独立内存存在，也就是，它不是内存！！它就是存信息的结构
    //但是本项目，那个node放进内存里了，所以本函数中，MemoryHeader* next不用malloc，放它的内存本来就存在
    int padding =0;
    
    if(size%4!=0)
    {
        padding = 4-size%4;
    }
    
    current->packed =(void*)current + sizeof(MemoryHeader) + size + padding;//current->size（set_size函数设置）和分配位未设置（set_allocated设置）
    //current->packed在设置后，以下两行效果一致
    //MemoryHeader* next = current->packed|1 - 1;//其实current->pack就够了
    //MemoryHeader* next = current + sizeof(MemoryHeader) + current->size + padding;//全是current数据。
}

void set_size(MemoryHeader *current, int size)
{
    current->size=size;
}

void set_allocated(MemoryHeader *current)
{
   current->packed = (MemoryHeader*)((long)current->packed|1);
}

void set_free(MemoryHeader *current)
{
    current->packed = (MemoryHeader*)(((long)current->packed|1)-1);
}

void coalesce(MemoryHeader *current, MemoryHeader *last)
{
    last->size = (void*)get_next_header(last) - (void*)last - sizeof(MemoryHeader)+ (void*)get_next_header(current) - (void*)current;
    last->packed = get_next_header(current);
    set_free(last);
    current = NULL;
    //last = realloc(last,last->size+sizeof(MemoryHeader));free(current);  不可以realloc和free，mem_init时内存已经分配过了，任何操作只是分块。见set_next_packet函数中解释
    
}

int mem_init(int size, enum Algorithm selectAlgo)
{
    algo = selectAlgo;
    all_len = size;
    
    memory_header = (MemoryHeader*)malloc(size);

    if(memory_header)//init的分配位为0
    {
        memory_header->packed = (void*)memory_header + size;
        memory_header->size = size - sizeof(MemoryHeader);
        set_free(memory_header);
        return 0;
    }
    else
    {
        return -1;
    }
    
}

void* mem_alloc_for_first_fit(int size)
{
   MemoryHeader* p = memory_header;
   MemoryHeader* q = memory_header;
   MemoryHeader* end = (void*)memory_header + all_len;

   while(p!=end)
   {
       if((is_free(p)==1) && (p->size >= size) && (get_next_header(p)!=end))
       {
          p->size = size;
          set_allocated(p);
          return get_data(p);
       }
       q=p;
       p = get_next_header(p);
   }

   if(q->size > size && (is_free(q)==1))//最后那个
   {
      
       set_next_packet(q,size);
       q->size = size;
       set_next_packet(q,size);
       set_allocated(q);
       MemoryHeader* new = get_next_header(q);
       new->packed = end;
       set_size(new,(long)end-(long)get_data(new));
       set_free(new);
       return get_data(q);
   }
   else
   {
       printf("Space is not enough.\n");
       return NULL;
   }

   return NULL;


   
}

void *mem_alloc(int size)
{
    switch (algo)
    {
    case FIRST_FIT://枚举：FIRST_FIT  字符串：“FIRST_FIT“
        return mem_alloc_for_first_fit(size);
        break;

    default:
        return NULL;
    }

}

int mem_free(void *data)
{
   int data_flag=-1;int flag=-1;
   MemoryHeader* p = memory_header;
   MemoryHeader* q = memory_header;
   MemoryHeader* end = (void*)memory_header + all_len;

   MemoryHeader* head=(MemoryHeader*)get_header_from_data(data);

   while(p!=end)
   {
      if(p==head)
      {
          data_flag=0;
          break;
      }

      p = get_next_header(p);
   }

   if(data_flag==-1)
   {
       return -1;
   }

   p = memory_header;

   while(p!=end)
   {
       if(get_data(p)==data)
       {
           set_free(p);
           flag=0;
       }

       p = get_next_header(p);
   }
   
   p = memory_header;
   
   do{
      q = get_next_header(p);

      if(q == end)
      {
          break;
      }

      printf("p:%p q:%p\n",p,q);

      if(is_free(q)==1 && is_free(p)==1)
      {
          printf("p:%p q:%p\n",p,q);
          coalesce(q, p);
          continue;
      }
      
      p = q;
   }while(p!=end);

   if(flag==0)
   {
       return 0;
   }

   return -1;
}

void mem_dump()
{
    printf("---------------------------------------------------------------------------------\n");
    printf("****************************MemoryHeader****************************************\n");
    printf("id   header    data      end       length    size   padding  used\n");

    MemoryHeader* p = memory_header;
    MemoryHeader* end = (void*)memory_header + all_len;
    int count = 1;
    
    do{
       int padding = (void*)get_next_header(p)-(void*)p-sizeof(MemoryHeader)-p->size;
       printf("%-5d%-10p%-10p%-10p%-10ld%-7d%-9d%-4d%-13p\n",count,p,get_data(p),(void*)get_next_header(p)-1,(void*)get_next_header(p)-(void*)p,p->size,padding,is_allocated(p),p->packed);
       count++;
       p = get_next_header(p);
    }while(p!=end);

    printf("****************************MemoryHeader****************************************\n");
}

