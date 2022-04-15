//头文件区域/////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//宏定义区域/////////////////////////////////////////////
//名字的长度
#define SIZE 16
//定量时间片
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
  head_task = NULL;
  head_waiting_time = NULL;
  head_sched = NULL;
}

/**
 * @brief 打印任务结构体信息
 * 结构体每个成员都打印出来，调试时方便使用
 *
 * @param task 任务结构体
 */
void print_task(Task *task)
{
  printf("Task:\n");
  printf("name\t\t%s\n", task->name);
  printf("priority\t%d\n", task->priority);
  printf("burst\t\t%d\n", task->burst);
  printf("arrival_time\t%d\n", task->arrival_time);
  printf("sched_sum\t%d\n", task->sched_sum);
  printf("waiting_time\t%d\n", task->waiting_time);
  printf("start_time\t%d\n", task->start_time);
  printf("end_time\t%d\n", task->end_time);
}

/**
 * @brief 插入一个新任务到链表中
 *
 * @param head 链表头
 * @param new_task 新任务
 */
void insert_node(struct Node **head, Task *new_task)
{
  struct Node *new_node = malloc(sizeof(struct Node));
  struct Node *temp = *head;

  new_node->next = NULL;
  new_node->task = NULL;

  if (NULL == temp)
  {
    new_node->task = new_task;
    *head = new_node;
    return;
  }

  while (NULL != temp->next)
  {
    temp = temp->next;
  }

  new_node->task = new_task;
  new_node->next = NULL;
  temp->next = new_node;
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
  struct Node *next;
  struct Node *prev;
  struct Node *temp;

  next = *head;

  if (strcmp(task->name, next->task->name) == 0)
  {
    temp = next;
    *head = (*head)->next;
    free(temp);
    return;
  }

  prev = *head;
  next = next->next;

  while (strcmp(task->name, next->task->name) != 0)
  {
    prev = next;
    next = next->next;
  }

  temp = next;
  prev->next = next->next;
  free(temp);
}

/**
 * @brief 释放链表中所有节点的资源（动态分配的内存资源）
 *
 * @param head 链表头
 */
void free_all_node(struct Node *head)
{
  struct Node *next;
  struct Node *temp;
  next = head;

  while (next != NULL)
  {
    temp = next;
    next = next->next;

    free(temp->task);
    free(temp);
  }
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
  struct Node *temp = head;

  while (temp != NULL)
  {
    count++;
    temp = temp->next;
  }

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
  struct Node *temp = head;

  while (temp != NULL)
  {
    if (0 == strcmp(task->name, temp->task->name))
    {
      if (task->sched_sum <= 1)
      {
        temp->task->waiting_time += slice - task->arrival_time;
        return;
      }

      temp->task->waiting_time += slice - task->end_time;
      return;
    }

    temp = temp->next;
  }
}

/**
 * @brief 打印等待时间
 *
 * @param head 链表头
 */
void print_waiting_time(struct Node *head)
{
  double sum = 0.00;
  struct Node *temp = head;

  printf("Waiting Time:\n");

  while (temp != NULL)
  {
    printf("%s\t%d\n", temp->task->name, temp->task->waiting_time);
    sum += temp->task->waiting_time;
    temp = temp->next;
  }

  printf("Average Waiting Time: %.2lf\n", sum / count_node(head));
}

/**
 * @brief 打印调度列表
 *
 * @param head 链表头
 */
void print_sched_list(struct Node *head)
{
  int count = 0;
  struct Node *temp = head;

  printf("Scheduling:\n");

  while (temp != NULL)
  {
    printf("%s\t%d\t%d\n", temp->task->name, temp->task->start_time, temp->task->end_time);
    count++;
    temp = temp->next;
  }
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
  Task *new_task = malloc(sizeof(Task));
  memset(new_task->name, 0, SIZE);
  memcpy(new_task->name, name, strlen(name));
  new_task->priority = priority;
  new_task->burst = burst;
  new_task->sched_sum = 0;
  new_task->arrival_time = arrival_time;
  new_task->start_time = 0;
  new_task->end_time = 0;
  new_task->waiting_time = 0;

  insert_node(&head_task, new_task);

  Task *new_task_waiting_time = malloc(sizeof(Task));
  memset(new_task_waiting_time->name, 0, SIZE);
  memcpy(new_task_waiting_time->name, name, strlen(name));
  new_task_waiting_time->waiting_time = 0;

  insert_node(&head_waiting_time, new_task_waiting_time);
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
  if (!head_task)
  {
    return NULL;
  }

  struct Node *temp;
  temp = head_task;
  Task *best_sofar = temp->task;

  delete_node(&head_task, best_sofar);
  return best_sofar;
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
  int slice_sum = 0;
  int slice_last = 0;
  int slice = 0;
  Task *task = pick_next_task_fsfs();

  printf("\n[FCFS]\n");

  while (NULL != task)
  {
    slice = task->burst;
    slice_last = slice_sum;
    slice_sum += slice;

    Task *new_task_sched = malloc(sizeof(Task));
    memset(new_task_sched->name, 0, SIZE);
    memcpy(new_task_sched->name, task->name, strlen(task->name));

    update_waiting_time_node(head_waiting_time, task, slice_last);
    task->start_time = slice_last;
    task->end_time = slice_sum;

    new_task_sched->start_time = slice_last;
    new_task_sched->end_time = slice_sum;
    insert_node(&head_sched, new_task_sched);

    free(task);
    task = pick_next_task_fsfs();
  }

  print_sched_list(head_sched);
  free_all_node(head_sched);

  print_waiting_time(head_waiting_time);
  free_all_node(head_waiting_time);
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
  if (!head_task)
  {
    return NULL;
  }

  struct Node *temp;
  temp = head_task;
  Task *best_sofar = temp->task;
  int best_tid = temp->task->sched_sum;

  while (temp != NULL)
  {
    if (!equals(best_tid, temp->task->sched_sum))
    {
      best_tid = temp->task->sched_sum;
      best_sofar = temp->task;
    }

    temp = temp->next;
  }

  add_tid(best_sofar);

  if (is_delete_node_rr(best_sofar->burst))
  {
    delete_node(&head_task, best_sofar);
  }

  return best_sofar;
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
  int slice_sum = 0;
  int slice_last = 0;
  int slice = 0;
  Task *task = pick_next_task_rr();

  printf("\n[RR]\n");

  while (NULL != task)
  {
    slice = get_slice_rr(task->burst);
    slice_last = slice_sum;
    slice_sum += slice;

    Task *new_task_sched = malloc(sizeof(Task));
    memset(new_task_sched->name, 0, SIZE);
    memcpy(new_task_sched->name, task->name, strlen(task->name));

    update_waiting_time_node(head_waiting_time, task, slice_last);
    task->start_time = slice_last;
    task->end_time = slice_sum;

    new_task_sched->start_time = slice_last;
    new_task_sched->end_time = slice_sum;
    insert_node(&head_sched, new_task_sched);

    if (is_delete_node_rr(task->burst))
    {
      free(task);
    }
    else
    {
      sub_slice(task, slice);
    }

    task = pick_next_task_rr();
  }

  print_sched_list(head_sched);
  free_all_node(head_sched);

  print_waiting_time(head_waiting_time);
  free_all_node(head_waiting_time);
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
  if (!head_task)
  {
    return NULL;
  }

  struct Node *temp;
  temp = head_task;
  Task *best_sofar = temp->task;
  int best_burst = temp->task->burst;

  while (temp != NULL)
  {
    if (min(temp->task->burst, best_burst))
    {
      if (temp->task->arrival_time <= time)
      {
        best_burst = temp->task->burst;
        best_sofar = temp->task;
      }
    }
    else if (equals(temp->task->burst, best_burst))
    {
      if (temp->task->arrival_time <= time)
      {
        best_sofar = temp->task;
      }
    }

    temp = temp->next;
  }

  add_tid(best_sofar);

  delete_node(&head_task, best_sofar);
  return best_sofar;
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
  int slice_sum = 0;
  int slice_last = 0;
  int slice = 0;
  Task *task = pick_next_task_nsjf(slice_sum);

  printf("\n[NSJF]\n");

  while (NULL != task)
  {
    slice = task->burst;
    slice_last = slice_sum;
    slice_sum += slice;

    Task *new_task_sched = malloc(sizeof(Task));
    memset(new_task_sched->name, 0, SIZE);
    memcpy(new_task_sched->name, task->name, strlen(task->name));

    update_waiting_time_node(head_waiting_time, task, slice_last);
    task->start_time = slice_last;
    task->end_time = slice_sum;

    new_task_sched->start_time = slice_last;
    new_task_sched->end_time = slice_sum;
    insert_node(&head_sched, new_task_sched);

    free(task);
    task = pick_next_task_nsjf(slice_sum);
  }

  print_sched_list(head_sched);
  free_all_node(head_sched);

  print_waiting_time(head_waiting_time);
  free_all_node(head_waiting_time);
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
  int slice = task->burst;

  if (!head_task)
  {
    return slice;
  }

  struct Node *temp = head_task;

  while (temp != NULL)
  {
    if (temp->task == task)
    {
      break;
    }

    temp = temp->next;
  }

  temp = temp->next;

  while (temp != NULL)
  {
    if (((temp->task->arrival_time + temp->task->burst) < (time + task->burst)) && time < temp->task->arrival_time)
    {
      slice = temp->task->arrival_time - task->arrival_time;
      return slice;
    }

    temp = temp->next;
  }

  return slice;
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
  if (!head_task)
  {
    return NULL;
  }

  struct Node *temp;
  temp = head_task;
  Task *best_sofar = temp->task;
  int best_burst = temp->task->burst;

  while (temp != NULL)
  {
    if (min(temp->task->burst, best_burst))
    {
      if (temp->task->arrival_time <= time)
      {
        best_burst = temp->task->burst;
        best_sofar = temp->task;
      }
    }
    else if (equals(temp->task->burst, best_burst))
    {
      if (temp->task->arrival_time <= time)
      {
        best_sofar = temp->task;
      }
    }

    temp = temp->next;
  }

  add_tid(best_sofar);

  *slice = get_slice_psjf(time, best_sofar);

  if (is_delete_node_psjf(*slice, best_sofar->burst))
  {
    delete_node(&head_task, best_sofar);
  }

  return best_sofar;
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
  int slice_sum = 0;
  int slice_last = 0;
  int slice = 0;
  Task *task = pick_next_task_psjf(slice_sum, &slice);

  printf("\n[PSJF]\n");

  while (NULL != task)
  {
    slice_last = slice_sum;
    slice_sum += slice;

    Task *new_task_sched = malloc(sizeof(Task));
    memset(new_task_sched->name, 0, SIZE);
    memset(new_task_sched->name, 0, strlen(task->name));
    memcpy(new_task_sched->name, task->name, strlen(task->name));

    update_waiting_time_node(head_waiting_time, task, slice_last);
    task->start_time = slice_last;
    task->end_time = slice_sum;

    new_task_sched->start_time = slice_last;
    new_task_sched->end_time = slice_sum;
    insert_node(&head_sched, new_task_sched);

    if (is_delete_node_psjf(slice, task->burst))
    {
      free(task);
    }
    else
    {
      sub_slice(task, slice);
    }

    task = pick_next_task_psjf(slice_sum, &slice);
  }

  print_sched_list(head_sched);
  free_all_node(head_sched);

  print_waiting_time(head_waiting_time);
  free_all_node(head_waiting_time);
}

/**
 * @brief 从文件中按行读取数据，解析处理，增加任务到链表中
 *
 * @param filename 输入文件名
 */
void input_data(char *filename)
{
  FILE *file;
  char *temp;
  char task[SIZE];

  char *name;
  int priority;
  int burst;
  int order = 0;

  file = fopen(filename, "r");

  while (fgets(task, SIZE, file) != NULL)
  {
    temp = strdup(task);
    name = strsep(&temp, ",");
    priority = atoi(strsep(&temp, ","));
    burst = atoi(strsep(&temp, ","));

    add_task(name, priority, burst, order++);

    free(temp);
  }

  fclose(file);
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
  file = freopen(filename, "w+", stdout);

  if (NULL == file)
  {
    printf("open file %s error.\n", filename);
    exit(1);
  }
}

/**
 * @brief 关闭文件
 *
 */
void output_close()
{
  if (NULL != file)
  {
    fclose(file);
  }
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
