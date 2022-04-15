//头文件区域/////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

//宏定义区域/////////////////////////////////////////////
//名字的长度
#define SIZE 16
//定量时间
#define QUANTUM 4
#define LINE_LEN 225
#define WORD_LEN 10

//结构体区域///////////////////////////////////////////////
/**
 * @brief 任务结构体
 *
 */
typedef struct
{
  //任务名字
  char name[SIZE];
  //编号
  int No;
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
FILE *file = NULL;
struct Node *head_task;//任务的链表头
struct Node *head_waiting_time;//等待时间的链表头
struct Node *head_sched;//调度的链表头

//函数区域///////////////////////////////////////////////
void print_sched_list(struct Node *head);
void init_head()//初始化链表头
{
   head_task=(struct Node*)malloc(sizeof(struct Node));
   head_task->next=NULL;
   head_task->task=NULL;

   head_waiting_time=(struct Node*)malloc(sizeof(struct Node));
   head_waiting_time->next=NULL;
   head_waiting_time->task=NULL;

   head_sched=(struct Node*)malloc(sizeof(struct Node));
   head_sched->next=NULL;
   head_waiting_time->task=NULL;
}

void print_task(Task *task)
{
  printf("name:%s priority:%d burst%d arrival_time%d\n",task->name,task->priority,task->burst,task->arrival_time);
  printf("start_time:%d end_time:%d waiting_time%d sched_sum%d\n",task->start_time,task->end_time,task->waiting_time,task->sched_sum);
  printf("\n");
}

void insert_node(struct Node **head, Task *new_task)
{
   struct Node *q = (*head);
   struct Node *p = (*head);

   while(q)
   {
      p=q;
      q=p->next;
   }
   
   struct Node *s = (struct Node*)malloc(sizeof(struct Node));//这里不能释放，和add_node的new_task一个原应
   s->task = new_task;
   s->next = NULL;
   p->next = s;
} 

bool delete_node(struct Node **head, Task *task)
{
   struct Node *p = (*head);
   struct Node *q = (*head);
   p = p->next;
   
   while(strcmp(p->task->name,task->name)!=0)
   {
      q=p;
      p = p->next;
   }

   if(p!=NULL)
   {
      q->next = p->next;
      free(p);
      return true;
   }
   else
   {
      return false;
   }
   
}

void free_all_node(struct Node *head)
{
   struct Node *p;
   
   while(head)
   {
      p = head;
      head = head->next;
      free(p);
   }
   
   /*以下为清空链表，即链表存在，只是没有节点
   struct Node *p = head->next;

   while(p)
   {
      struct Node *q = p->next;
      free(p);
      p = q;
   }

   head->next = NULL;*/
}

struct Node* find_node(struct Node *head , char* name)
{
   struct Node *p = head->next;

   while(p)
   {
     if(strcmp(name,p->task->name)==0)
     {
        return p;
     }
     
     p=p->next;
   }

   return NULL;
}

int count_node(struct Node *head)
{
  int count = 0;
  struct Node *p = head->next;

   while(p)
   {
      p=p->next;
      count++;
   }

  return count;
}

void update_waiting_time_node(struct Node *head, Task *task, int slice);

void print_waiting_time(struct Node *head)
{
  int time = head->task->end_time - head->task->burst - head->task->start_time;
  printf("waiting time:%d\n",time);
}

void print_sched_list(struct Node *head)
{
   struct Node *p = head->next;

   while(p)
   {
      print_task(p->task);
      p = p->next;
   }
}

void add_task(char *name, int priority, int burst, int arrival_time)
{
   Task* new_task = (Task*)malloc(sizeof(Task));
   if(new_task==NULL)
   {
     perror("add_task");
     exit(1);
   }
   
   strcpy(new_task->name,name);
   new_task->priority = priority;
   new_task->burst = burst;
   new_task->arrival_time = arrival_time;
   new_task->end_time = 0;
   new_task->start_time = -1;
   new_task->waiting_time = 0;
   new_task->sched_sum = 0;
   new_task->No = -1;
   insert_node(&head_task, new_task);
   //free(new_task);不能有！insert_node传进去的是new_task，是个指针，全靠这里的值撑着
}

void add_sched_task(char *name, int now_time, int add, int sched_sum)
{
   Task* new_task = (Task*)malloc(sizeof(Task));
   if(new_task==NULL)
   {
     perror("add_task");
     exit(1);
   }
   
   strcpy(new_task->name,name);
   new_task->priority = -1;
   new_task->burst = -1;
   new_task->arrival_time = -1;
   new_task->end_time = now_time+add;
   new_task->start_time = now_time;
   new_task->waiting_time = -1;
   new_task->sched_sum = sched_sum;
   new_task->No = -1;
   insert_node(&head_sched, new_task);
   
}

void rank_by_now_priority_rule(int* priority , char** name_rank)
{
   Task *task;
   struct Node *p;
   int i=0;
   int node_num = count_node(head_task);
   int temp;
  
   p = head_task->next;
   while(p && i<node_num)
   {
      strcpy(name_rank[i],p->task->name);//name_rank[i]必须malloc
      p = p->next;
      i++;
   }

   char *temp_name = (char*)malloc(sizeof(char)*WORD_LEN);
   
   for(int j=0;j<node_num-1;j++)
   {
      for(int m=0;m<node_num-1-j;m++)
      {
          if(priority[m]>priority[m+1])
          {
             temp = priority[m+1];
             priority[m+1] = priority[m];
             priority[m] = temp;
             strcpy(temp_name,name_rank[m+1]);
             strcpy(name_rank[m+1],name_rank[m]);
             strcpy(name_rank[m],temp_name);
          }
      }
   }

   for(int j=0;j<node_num;j++)
   {
      p = find_node(head_task ,name_rank[j]);
      insert_node(&head_waiting_time, p->task);//注意，和head_task用的一个task
   }

}

// FCFS

void schedule_task_fcfs()
{
   Task *task;
   struct Node *p;
   int i=0;
   int node_num = count_node(head_task);
   int priority[node_num];
   char* name_rank[node_num];

   for(int j=0;j<node_num;j++)
   {
      name_rank[j]=(char*)malloc(sizeof(char)*WORD_LEN);
   }
  
   p = head_task->next;
   while(p && i<node_num)
   {
      task = p->task;
      priority[i]=task->arrival_time;
      p = p->next;
      i++;
   }

   rank_by_now_priority_rule(priority,name_rank);

   p = head_waiting_time->next;
   int now_time=0;
   while(p)
   {
      p->task->start_time = now_time;
      p->task->end_time = now_time + p->task->burst;
      now_time = p->task->end_time;
      p = p->next;
   }
   
   p = head_waiting_time->next;
   while(p)
   {
      p->task->waiting_time = p->task->end_time - p->task->burst - p->task->arrival_time;
      insert_node(&head_sched, p->task);
      p = p->next;
   }
   
   printf("FCFS 1\n\n");
   printf("head_task\n\n");
   print_sched_list(head_sched);
}

// RR

void schedule_task_rr()
{
  Task *task;
   struct Node *p;
   int i=0;
   int j=0;
   int now_time=0;
   int node_num = count_node(head_task);
   int priority[node_num];
   char* name_rank[node_num];

   for(int j=0;j<node_num;j++)
   {
      name_rank[j]=(char*)malloc(sizeof(char)*WORD_LEN);
   }
  
   p = head_task->next;
   while(p && i<node_num)
   {
      task = p->task;
      priority[i]=task->arrival_time;
      p = p->next;
      i++;
   }

   rank_by_now_priority_rule(priority,name_rank);

   struct Node *q = head_waiting_time;
   p = head_waiting_time;
   struct Node *m;
   while((q->next)!=NULL)
   {
      if((p->next) == NULL)
      {
         m = head_waiting_time;
         p = head_waiting_time->next;
      }
      else
      {
         m = p;
         p = p->next;
      }

      int rest = p->task->burst - p->task->sched_sum;
      int add = ((rest < QUANTUM) ? rest : QUANTUM);
      p->task->sched_sum += add;
      add_sched_task(p->task->name, now_time, add, p->task->sched_sum);

      if(p->task->start_time == -1)
      {
         p->task->start_time = now_time;
         
      }
     
      now_time += add;
     
      if(p->task->sched_sum == p->task->burst)
      {
         p->task->end_time = now_time;
         p->task->waiting_time = p->task->end_time - p->task->burst - p->task->arrival_time;
         delete_node(&head_waiting_time,p->task); 
         p = m;
      }
   }

   printf("RR 2\n\n");
   printf("head_task\n\n");
   print_sched_list(head_task);
   printf("head_sched\n\n");
   print_sched_list(head_sched);
}

//NSJF

void schedule_task_nsjf()
{
   Task *task;
   struct Node *p;
   int i=0;
   int j=0;
   int now_time=0;
   int node_num = count_node(head_task);
   int priority[node_num];
   char* name_rank[node_num];

   for(int j=0;j<node_num;j++)
   {
      name_rank[j]=(char*)malloc(sizeof(char)*WORD_LEN);
   }
  
   p = head_task->next;
   while(p && i<node_num)
   {
      task = p->task;
      priority[i]=task->burst;
      p = p->next;
      i++;
   }

   rank_by_now_priority_rule(priority,name_rank);

   for(int j=0;j<node_num;j++)
   {
      free(name_rank[j]);
   }
   
   struct Node *q = head_waiting_time;
   p = head_waiting_time;
   struct Node *m;
   int flag =0;
   while((q->next)!=NULL)
   {
      if((p->next) == NULL)
      {
         m = head_waiting_time;
         p = head_waiting_time->next;
      }
      else
      {
         m = p;
         p = p->next;
      }

      if(p->task->arrival_time <= now_time)
      {
         p->task->start_time = now_time;
         p->task->end_time = now_time + p->task->burst;
         p->task->waiting_time = p->task->end_time - p->task->burst - p->task->arrival_time;
         now_time = p->task->end_time;
         delete_node(&head_waiting_time,p->task); 
         
         if(flag==1)
         {
            p = head_waiting_time;
            flag=0;
         }
         else
         {
            p = m;
         }
      }
      else
      {
         p = p->next;
         flag = 1;
      }
   }

   printf("NSJF 3\n\n");
   printf("head_task\n\n");
   print_sched_list(head_task);
   
}

//PSJF

void schedule_task_psjf()
{
   Task *task;
   struct Node *p;
   int i=0;
   int j=0;
   int now_time=0;
   int node_num = count_node(head_task);
   int priority[node_num];
   int decision_point[node_num];
   int sched_sum[node_num];
   char* name_rank[node_num];

   for(int j=0;j<node_num;j++)
   {
      name_rank[j]=(char*)malloc(sizeof(char)*WORD_LEN);
   }
  
   p = head_task->next;
   while(p && i<node_num)
   {
      //p->task->No = i;
      priority[i]=p->task->burst;
      decision_point[i]=p->task->arrival_time;
      strcpy(name_rank[i],p->task->name);
      sched_sum[i]=0;
      //No_rank[i] = i;
      p = p->next;
      i++;
   }
   
   char *temp_name = (char*)malloc(sizeof(char)*WORD_LEN);
   for(int j=0;j<node_num-1;j++)
   {
      for(int m=0;m<node_num-1-j;m++)
      {
          if(decision_point[m]>decision_point[m+1])
          {
             int temp = decision_point[m+1];
             decision_point[m+1] = decision_point[m];
             decision_point[m] = temp;
             strcpy(temp_name,name_rank[m+1]);
             strcpy(name_rank[m+1],name_rank[m]);
             strcpy(name_rank[m],temp_name);
             int temp_priority = priority[m+1];
             priority[m+1] = priority[m];
             priority[m] = temp_priority;
             int temp_sched_sum = sched_sum[m+1];
             sched_sum[m+1] = sched_sum[m];
             sched_sum[m] = temp_sched_sum;
          }
      }
   }

   for(int j=0;j<node_num;j++)
   {
      now_time = decision_point[j];

      if(j==0)
      {
         p = find_node(head_task , name_rank[0]);
         p->task->start_time = decision_point[0];
         add_sched_task(name_rank[0], decision_point[0], decision_point[1]-decision_point[0], decision_point[1]-decision_point[0]);
         sched_sum[0] = decision_point[1]-decision_point[0];
         priority[0] = priority[0] - (decision_point[1]-decision_point[0]);
         

         if(priority[0]==0)
         {
            
            p->task->end_time = decision_point[j+1] ; 
            p->task->waiting_time = p->task->end_time - p->task->burst - p->task->arrival_time;
         }
      }
      else
      {
         for(int n=0;n<j;n++)
         {
            for(int m=0;m<j-n;m++)
            {
                if(priority[m]>priority[m+1])
                {
                    strcpy(temp_name,name_rank[m+1]);
                    strcpy(name_rank[m+1],name_rank[m]);
                    strcpy(name_rank[m],temp_name);
                    int temp_priority = priority[m+1];
                    priority[m+1] = priority[m];
                    priority[m] = temp_priority;
                    int temp_sched_sum = sched_sum[m+1];
                    sched_sum[m+1] = sched_sum[m];
                    sched_sum[m] = temp_sched_sum;
                }  
            }
         }
         
         p = find_node(head_task , name_rank[0]);

         if(sched_sum[0]==0)
         {
           p->task->start_time = now_time;
         }
         
         if(j<node_num-1)
         {
            sched_sum[0] = sched_sum[0] + decision_point[j+1]-decision_point[j];
            priority[0] = priority[0] - (decision_point[j+1]-decision_point[j]);
            add_sched_task(name_rank[0], decision_point[j], decision_point[j+1]-decision_point[j], sched_sum[0]);

            if(priority[0]==0)
            {
               p->task->end_time = decision_point[j+1] ; 
               p->task->waiting_time = p->task->end_time - p->task->burst - p->task->arrival_time;
            }
         }
         else
         {
            now_time = decision_point[node_num-1] + priority[0];
            sched_sum[0] = sched_sum[0] + priority[0];
            add_sched_task(name_rank[0], decision_point[node_num-1], priority[0], priority[0] + sched_sum[0]);
            priority[0] = 0;
            p->task->end_time = now_time ; 
            p->task->waiting_time = p->task->end_time - p->task->burst - p->task->arrival_time;
         }

      }
   }

   for(int n=0;n<node_num-1;n++)
   {
      for(int m=0;m<node_num-1-n;m++)
      {
         if(priority[m]>priority[m+1])
         {
            strcpy(temp_name,name_rank[m+1]);
            strcpy(name_rank[m+1],name_rank[m]);
            strcpy(name_rank[m],temp_name);
            int temp_priority = priority[m];
            priority[m+1] = priority[m];
            priority[m] = temp_priority;
            int temp_sched_sum = sched_sum[m];
            sched_sum[m+1] = sched_sum[m];
            sched_sum[m] = temp_sched_sum;
         }  
      }
   }

   for(int j=0;j<node_num;j++)
   {
      if (priority[j]==0)
      {
         continue;
      }
      
      p = find_node(head_task , name_rank[j]);

      if(sched_sum[j]==0)
      {
        p->task->start_time = now_time;
      }

      sched_sum[j] = sched_sum[j]+priority[j];
      add_sched_task(name_rank[j], now_time, priority[j], sched_sum[j]);
      now_time = now_time + priority[j];
      priority[j]=0;
      p->task->end_time = now_time ; 
      p->task->waiting_time = p->task->end_time - p->task->burst - p->task->arrival_time;

   }
   
   printf("PSJF 4\n\n");
   printf("head_task\n\n");
   print_sched_list(head_task);
   printf("head_sched\n\n");
   print_sched_list(head_sched);
   
}

void input_data(char *filename)
{
    FILE* fp;
    char line[LINE_LEN];
    char name[WORD_LEN];
    char priority[WORD_LEN];
    char burst[WORD_LEN];
    int task_count=-1;

    fp=fopen(filename,"r");

    if (fp==NULL)
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }

    for(int i=0;i<WORD_LEN;i++)
    {
       name[i] = '\0';
       priority[i] = '\0';
       burst[i] = '\0';
    }

    while(fgets(line,LINE_LEN,fp)!=NULL)
    {
       int len = strlen(line);
       task_count++;
       int count = 0;
       int char_count=0;

       for(int i=0;i<len-1;i++)
       {
          if(line[i]!=',')
          {
            if(count==0)
            {
              name[char_count++]=line[i];
            }
            else if(count==1)
            {
              priority[char_count++]=line[i];
            }
            else if(count==2)
            {
              burst[char_count++]=line[i];
            }
          }
          else
          {
            count++;
            char_count=0;
          }
       }

       add_task(name, atoi(priority), atoi(burst), task_count);//警惕，name传的地址，这个值直到malloc才会被拷贝到别的地方，期间乱动会出事
       memset(name,'\0', WORD_LEN);
       memset(priority,'\0', WORD_LEN);
       memset(burst,'\0', WORD_LEN);
    }

    fclose(fp);
}

void output_open(char *filename)
{
    if(freopen(filename,"w",stdout)==NULL)
    {
        fprintf(stderr,"output_open:%s\n",(strerror(errno)));
        exit(EXIT_FAILURE);
    }
    
}

void output_close()
{
  fclose(stdout);
}

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
   usage(argc);
   output_open(argv[2]);
   init_head();
   input_data(argv[1]);
   printf("orgin\n\n");
   print_sched_list(head_task);
   //schedule_task_fcfs();
   //schedule_task_rr();
   //schedule_task_nsjf();
   schedule_task_psjf();
   output_close();

}
