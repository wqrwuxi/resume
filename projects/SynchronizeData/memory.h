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
#ifndef MEMORY_H
#define MEMORY_H

#pragma pack(4)

/**
 * @brief 算法
 * 
 */
enum Algorithm
{
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT,
    NEXT_FIT
};

/**
 * @brief 初始化内存分配
 * 一次初始化.
 * 
 * @param size 指定需要分配的块的大小
 * @param algo 算法
 * @return int 成功返回0, 失败返回-1
 */
int mem_init(int size, enum Algorithm algo);

/**
 * @brief 内存分配
 * 
 * 根据合适的算法来分配。
 * 下一个首部必须与可被4整除的地址对齐，需要考虑填充问题。
 * 空闲块的最小有效负载大小必须为4字节。
 * 
 * @param size 大小
 * @return void* 成功则返回已分配块的数据可写地址；失败时返回NULL
 */
void *mem_alloc(int size);

/**
 * @brief 释放内存
 * 
 * 如果ptr未指向分配块的第一个字节，则返回-1
 * 检查所有块头，确定是否设置了alloc位，将块标记为未分配状态
 * 如果一个或两个近邻都是未分配状态的，则合并
 * 
 * @param ptr 数据可写地址
 * @return int 成功返回0, 失败返回-1
 */
int mem_free(void *data);

/**
 * @brief 内存信息调试
 * 
 * 打印出所有块的列表，以及每个块的以下信息：
 * id: 内存块的序列号
 * header: 块头的地址
 * data: 数据可读写的地址(有效载荷的开始)
 * end: 块中最后一个字节的地址（有效负载或填充）
 * length: 块的总大小（包括首部、有效负载和填充）
 * size: 块的大小或有效载荷大小
 * padding: 块的填充大小
 * used： 状态（0或1）
 */
void mem_dump();

#endif // MEMORY_H
