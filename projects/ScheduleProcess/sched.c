//头文件区域/////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//宏定义区域/////////////////////////////////////////////
//名字的长度
#define SIZE 16
//定量时间
#define QUANTUM 4

//结构体区域///////////////////////////////////////////////
/**
 * @brief 任务结构体
 *
 */
typedef struct
{
  //任务名字
  char name[SIZE];
  //优先级
  int priority;
  //爆发
  int burst;
  //到达时间
  int arrival_time;
  //调度总量
  int sched_sum;
  //等待时间
  int waiting_time;
  //开始时间
  int start_time;
  //结束时间
  int end_time;
} Task;

/**
 * @brief 节点结构体
 *
 */
struct Node
{
  //任务
  Task *task;
  //下一个节点
  struct Node *next;
} Node;

//变量区域///////////////////////////////////////////////
/**
 * @brief 文件指针
 *
 */
FILE *file = NULL;

/**
 * @brief 任务的链表头
 *
 */
struct Node *head_task;

/**
 * @brief 等待时间的链表头
 *
 */
struct Node *head_waiting_time;

/**
 * @brief 调度的链表头
 *
 */
struct Node *head_sched;

//函数区域///////////////////////////////////////////////
/**
 * @brief 初始化链表头
 *
 */
void init_head()
{
  // TODO
}

/**
 * @brief 打印任务结构体信息
 * 结构体每个成员都打印出来，调试时方便使用
 *
 * @param task 任务结构体
 */
void print_task(Task *task)
{
  // TODO
}

/**
 * @brief 插入一个新任务到链表中
 *
 * @param head 链表头
 * @param new_task 新任务
 */
void insert_node(struct Node **head, Task *new_task)
{
  // TODO
}

/**
 * @brief 从链接中删除一个任务
 * 根据任务名比较，相同则删除
 *
 * @param head 链表头
 * @param task 任务
 */
void delete_node(struct Node **head, Task *task)
{
  // TODO
}

/**
 * @brief 释放链表中所有节点的资源（动态分配的内存资源）
 *
 * @param head 链表头
 */
void free_all_node(struct Node *head)
{
  // TODO
}

/**
 * @brief 计算有几个节点
 *
 * @param head 节点头
 * @return int 返回节点个数
 */
int count_node(struct Node *head)
{
  int count = 0;
  // TODO

  return count;
}

/**
 * @brief 更新等待时间
 *
 * @param head 链表头
 * @param task 任务
 * @param slice 时间片
 */
void update_waiting_time_node(struct Node *head, Task *task, int slice)
{
  // TODO
}

/**
 * @brief 打印等待时间
 *
 * @param head 链表头
 */
void print_waiting_time(struct Node *head)
{
  // TODO
}

/**
 * @brief 打印调度列表
 *
 * @param head 链表头
 */
void print_sched_list(struct Node *head)
{
  // TODO
}

/**
 * @brief 增加任务到链表中
 *
 * @param name 任务名字
 * @param priority 优先级
 * @param burst 爆发时间
 * @param arrival_time 到达时间
 */
void add_task(char *name, int priority, int burst, int arrival_time)
{
  // TODO
}

/**
 * @brief 比较前者是否小于后者
 *
 * @param src
 * @param dest
 * @return true
 * @return false
 */
bool min(int src, int dest) { return (src < dest); }

/**
 * @brief 比较两者是否相等
 *
 * @param src
 * @param dest
 * @return true
 * @return false
 */
bool equals(int src, int dest) { return (src == dest); }

/**
 * @brief 增加tid
 *
 * @param task
 */
void add_tid(Task *task) { task->sched_sum += 1; }

/**
 * @brief 减少时间片
 *
 * @param task
 * @param slice
 */
void sub_slice(Task *task, int slice) { task->burst -= slice; }

// FCFS

/**
 * @brief 取下一个任务（FCFS）
 *
 * @see delete_node
 *
 * @return Task*
 */
Task *pick_next_task_fsfs()
{
  // TODO
  return NULL;
}

/**
 * @brief 调度任务（FCFS）
 *
 * @see pick_next_task_fsfs
 * @see update_waiting_time_node
 * @see insert_node
 * @see print_sched_list
 * @see free_all_node
 * @see print_waiting_time
 *
 */
void schedule_task_fcfs()
{
  // TODO
}

// RR

/**
 * @brief 获取定量时间 （RR）
 *
 * @param burst
 * @return int
 */
int get_slice_rr(int burst) { return ((burst < QUANTUM) ? burst : QUANTUM); }

/**
 * @brief 是否要删除任务（RR）
 *
 * @param burst
 * @return true
 * @return false
 */
bool is_delete_node_rr(int burst) { return (burst <= QUANTUM); }

/**
 * @brief 取下一个任务（RR）
 *
 * @see equals
 * @see add_tid
 * @see is_delete_node_rr
 * @see delete_node
 *
 * @return Task*
 */
Task *pick_next_task_rr()
{
  // TODO
  return NULL;
}

/**
 * @brief 调度任务（RR）
 *
 * @see pick_next_task_rr
 * @see get_slice_rr
 * @see update_waiting_time_node
 * @see insert_node
 * @see is_delete_node_rr
 * @see sub_slice
 * @see print_sched_list
 * @see free_all_node
 * @see print_waiting_time
 *
 */
void schedule_task_rr()
{
  // TODO
}

// NSJF

/**
 * @brief 取下一个任务（NSJF）
 *
 * @see min
 * @see equals
 * @see add_tid
 * @see delete_node
 *
 * @param time
 * @return Task*
 */
Task *pick_next_task_nsjf(int time)
{
  // TODO
  return NULL;
}

/**
 * @brief 调度任务（NSJF）
 *
 * @see pick_next_task_nsjf
 * @see update_waiting_time_node
 * @see insert_node
 * @see print_sched_list
 * @see free_all_node
 * @see print_waiting_time
 *
 */
void schedule_task_nsjf()
{
  // TODO
}

// PSJF

/**
 * @brief 获取时间片 （PSJF）
 *
 * @param time
 * @param task
 * @return int
 */
int get_slice_psjf(int time, Task *task)
{
  // TODO
  return 0;
}

/**
 * @brief 是否从链表中删除任务（PSJF）
 *
 * @param slice
 * @param burst
 * @return true
 * @return false
 */
bool is_delete_node_psjf(int slice, int burst) { return (slice >= burst); }

/**
 * @brief 取下一个任务（PSJF）
 *
 * @see min
 * @see equals
 * @see add_tid
 * @see get_slice_psjf
 * @see is_delete_node_psjf
 * @see delete_node
 *
 * @param time
 * @return Task*
 */
Task *pick_next_task_psjf(int time, int *slice)
{
  // TODO

  return NULL;
}

/**
 * @brief 调度任务（PSJF）
 *
 * @see pick_next_task_psjf
 * @see update_waiting_time_node
 * @see insert_node
 * @see is_delete_node_psjf
 * @see sub_slice
 * @see print_sched_list
 * @see free_all_node
 * @see print_waiting_time
 *
 */
void schedule_task_psjf()
{
  // TODO
}

/**
 * @brief 从文件中按行读取数据，解析处理，增加任务到链表中
 *
 * @param filename 输入文件名
 */
void input_data(char *filename)
{
  // TODO
}

/**
 * @brief 打开输出文件
 *
 * 这里需要用到freopen，把标准控制台的输出重定向到文件。
 * 我们为了方便在输出到标准控制台和文件之间切换，这次采用这个方式。
 *
 *
 * @param filename 输出文件名
 */
void output_open(char *filename)
{
  // TODO
}

/**
 * @brief 关闭文件
 *
 */
void output_close()
{
  // TODO
}

/**
 * @brief 用法
 *
 * 如果命令行输入参数个数不符合要求，则打印用法，退出程序。
 *
 * @param argc 参数个数
 */
void usage(int argc)
{
  if (3 != argc)
  {
    printf("Usage: ./sched task.txt result.txt\n");
    exit(1);
  }
}

//主函数区域///////////////////////////////////////////////
/**
 * @brief 主函数入口
 *
 * @param argc 参数个数
 * @param argv 参数值数组
 * @return int 返回0为正常，其他为错误
 */
int main(int argc, char *argv[])
{
  //用法
  usage(argc);

  //注意：如果想输出到控制台，则需要注释掉这个函数
  output_open(argv[2]);

  // FCFS
  init_head();
  input_data(argv[1]);
  schedule_task_fcfs();

  // RR
  init_head();
  input_data(argv[1]);
  schedule_task_rr();

  // NSJF
  init_head();
  input_data(argv[1]);
  schedule_task_nsjf();

  // PSJF
  init_head();
  input_data(argv[1]);
  schedule_task_psjf();

  //关闭文件
  output_close();

  return 0;
}
