/**
 * @file sort.c
 * @author binary (binary@techbootcamp)
 * @brief 多线程排序项目
 * @version 0.1
 * @date 2021-11-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
///////////////////////////////////////////////////////////////////////////////
//头文件区域，所需要的头文件，添加到这里。

#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
//宏定义

#define SIZE 500

///////////////////////////////////////////////////////////////////////////////
//结构体

typedef struct
{
    int size;
    int data[SIZE];
} datas;

typedef struct
{
    int start;
    int end;
} parameters;

///////////////////////////////////////////////////////////////////////////////
//变量声明

///////////////////////////////////////////////////////////////////////////////
//函数声明

/**
 * @brief 打印数据
 * 
 * @param array 
 */
void print_data(datas array);

/**
 * @brief 检测数据是否已经排序
 * 
 * @param array 
 * @return true 
 * @return false 
 */
int is_sorted(datas array);

/**
 * @brief 排序线程的回调函数
 * 
 * @param params 
 * @return void* 
 */
void *sorter(void *params);

/**
 * @brief 合并线程的回调函数
 * 
 * @param params 
 * @return void* 
 */
void *merger(void *params);

/**
 * @brief 从文件中读取数据。（原始数据，未排序的）
 * 
 */
void input_data();

/**
 * @brief 排序线程的初始化和工作过程
 * 
 */
void handle_sorting();

/**
 * @brief 合并线程的初始化和工作过程
 * 
 */
void handle_merge();

/**
 * @brief 显示排序数据是否正确
 * 
 * 排序不正确，显示排序不正确，并打印出排序数据。
 * 排序正确，则显示排序正确。
 * 
 */
void show_data();

/**
 * @brief 输出数据到文件。（已经排好序的）
 * 
 */
void output_data();

/**
 * @brief 清理一些资源。比如动态内存分配的资源
 * 
 */
void cleanup();

//TODO：你的排序算法函数在这里声明

///////////////////////////////////////////////////////////////////////////////
//函数实现

/**
 * @brief 主函数人口
 * 
 * @param argc 参数个数
 * @param argv 参数数组
 * @return int 返回状态
 */
int main(int argc, char *argv[])
{
    input_data();
    handle_sorting();
    handle_merge();
    show_data();
    output_data();
    cleanup();
    return 0;
}

void print_data(datas array)
{
    printf("TODO: print_data\n");
}

int is_sorted(datas array)
{
    printf("TODO: is_sorted\n");

    return 0;
}

void *sorter(void *params)
{
    printf("TODO: sorter\n");
}

void *merger(void *params)
{
    printf("TODO: merger\n");
}

void input_data()
{
    printf("TODO: input_data\n");
}

void handle_sorting()
{
    printf("TODO: handle_sorting\n");
}

void handle_merge()
{
    printf("TODO: handle_merge\n");
}

void show_data()
{
    printf("TODO: show_data\n");
}

void output_data()
{
    printf("TODO: output_data\n");
}

void cleanup()
{
    printf("TODO: cleanup\n");
}

//TODO：你的排序算法函数在这里实现