//最终自己的v3
//最致命的错在188行，原来是if ((pidd[m] = fork()) < 0) 原来的，导致fork两次
//重申：fork几次就是几个子进程，并且父进程只有一个
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>
#include <stdbool.h>
#include <sys/times.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <errno.h>
#include <sys/wait.h>

#define SIZE    16

struct size_and_data {
    int size;
    int *data;
};

struct bin_info {
    int size;
    int *data;
};

struct thread_args {
    struct size_and_data array; // 哪一部分的数据要处理
    struct bin_info *bins;
};

pthread_mutex_t mutex ;

void print_data(struct size_and_data array) {
    for (int i = 0; i < array.size; ++i)
        printf("%d ", array.data[i]);
    printf("\n");
}

/* 检查数据是否已经排序正确*/
bool is_sorted(struct size_and_data array) {
    bool sorted = true;
    for (int i = 0; i < array.size - 1; i++) {
        if (array.data[i] > array.data[i + 1])
            sorted = false;
    }
    return sorted;
}

/* 随机生成数据去填充数组 */
void produce_random_data(struct size_and_data array) {
    srand(1); // 每次产生的随机数一样
    for (int i = 0; i < array.size; i++) {
        array.data[i] = rand() % 1000;
    }
}

/* 拆分数据到某个 bins*/
void split_data(struct size_and_data array, struct bin_info bins[])
{
    for (int i = 0; i < array.size; i++)
    {
        int number = array.data[i];
        if (number < 250)
        {
            bins[0].data[bins[0].size++] = number;
        }
        else if (number < 500)
        {
            bins[1].data[bins[1].size++] = number;
        }
        else if (number < 750)
        {
            bins[2].data[bins[2].size++] = number;
        }
        else
        {
            bins[3].data[bins[3].size++] = number;
        }
    }
}

/* 分配空间 */
int *allocate(int size) {
    int *space;
    space = (int *)calloc(size, sizeof(int));
    if (space == NULL) {
        perror("Problem allocating memory.\n");
        exit(EXIT_FAILURE);
    }
    return space;
}

/* 移动bins里面的数据到原始数组 */
void move_back(struct size_and_data array, struct bin_info bins[]) {
    for (int bin = 0; bin < 4; bin++) {
        for (int i = 0; i < bins[bin].size; i++) {
            *array.data++ = bins[bin].data[i];
        }
    }
}

/* 插入排序 */
void insertion(struct bin_info bin) {
    for (int i = 1; i < bin.size; i++) {
        for (int j = i; j > 0; j--) {
            if (bin.data[j-1] > bin.data[j]) {
                int temp;
                temp = bin.data[j];
                bin.data[j] = bin.data[j-1];
                bin.data[j-1] = temp;
            } else {
                break;
            }
        }
    }

    /*for (int i = 0; i < bin.size; i++) {
        printf("%d ",bin.data[i]);
    }  
     printf("bin\n"); */
}

/* 主函数 */
int main(int argc, char *argv[]) {
    struct size_and_data the_array;
    struct bin_info bins[4];
    struct thread_args args[4];
    pid_t	pid;
    pid_t   child_pid;
    pid_t	pidd[4];
    int fd[4][2];
    int fd_main[4][2];
    int status[4];
    
	if (argc < 2) {
		the_array.size = SIZE;
	} else {
		the_array.size = pow(2, atoi(argv[1]));;
	}

    the_array.data = allocate(the_array.size);
    for (int i = 0; i < 4; i++) {
        bins[i].size = 0;
        bins[i].data = allocate(the_array.size);
    }

    produce_random_data(the_array);

    if (the_array.size < 1025)
        print_data(the_array);

    struct tms start_times, finish_times;
    time_t start_clock, finish_clock;

    start_clock = time(NULL);
    times(&start_times);
    printf("start time in clock ticks: %ld\n", start_times.tms_utime);

    split_data(the_array, bins);

    int sum = 0;
    for (int i = 0; i < 4; i++) {
        sum += bins[i].size;
    }
    printf("Total size of bins: %d\n", sum);
    
    /*int test=getpid();
    printf("main_pid before loop: %d\n",test);*/

    for(int m=0;m<4;m++)
    {   
        if(pipe(fd[m])<0)
        {
            printf("Error in pipe %d: %s\n",m,(strerror(errno)));
        }

        int test1=getpid();
        printf("before fork:%d\n",test1);

        pidd[m] = fork();

        //int test2=getpid();
        //printf("after fork:%d\n",test2);
        
        //if ((pidd[m] = fork()) < 0) 原来的，导致两个fork不多出几个fork就见鬼了，而且一开始就有
        if (pidd[m] < 0)
        {
		  perror("fork error\n");
          exit(1);
	    } 
        else if(pidd[m]==0)
        {
            struct tms fork_start_times, fork_finish_times;
            time_t fork_start_clock, fork_finish_clock;

            fork_start_clock = time(NULL);
            times(&fork_start_times);
            printf("fork%d start time in clock ticks: %ld\n",m,fork_start_times.tms_utime);
            
            //printf("m %d, child_pid %d\n",m,pidd[m]);
            insertion(bins[m]);
            close(fd[m][0]);
            write(fd[m][1],bins[m].data,sizeof(int)*bins[m].size);//标准答案write改成read的样子也不行
            close(fd[m][1]);

            times(&fork_finish_times);
            fork_finish_clock = time(NULL);
            printf("fork%d finish time in clock ticks: %ld\n",m,fork_finish_times.tms_utime);
            printf("fork%d Total elapsed time in seconds: %ld\n",m,(fork_finish_clock - fork_start_clock));
            exit(0);

        }
        else if(pidd[m]>0)
        {   
        int test2=getpid();
        printf("m, parent_pid, child_pid %d:%d:%d\n",m,test2,pidd[m]);
            close(fd[m][1]);
            int pos = 0;
            int size = 0;
            while ((size = read(fd[m][0], bins[m].data + pos / sizeof(int), sizeof(int)*bins[m].size)) > 0)//测bug经验：觉得数据结构有问题才导致差异，换成自己的数据结构就好了
            {                                                                                                       //或者bins[i].data强行赋值
                printf("child [%d] pipe pos=%d size=%d\n", m, pos, size);
                pos += size;
            };
            close(fd[m][0]);
        }
    }

    int test1=getpid();
    printf("main_pid-after loop: %d\n",test1);
    
    for (int i = 0; i < 4; i++)
    {
        waitpid(pidd[i], &status[i], 0);
    }

    /*int test3=getpid();
    printf("main_pid-after waitpid: %d\n",test3);*/

    move_back(the_array, bins);

    times(&finish_times);
    finish_clock = time(NULL);
    printf("finish time in clock ticks: %ld\n", finish_times.tms_utime);
    printf("Total elapsed time in seconds: %ld\n", finish_clock - start_clock);

    if (the_array.size < 1025)
        print_data(the_array);

    printf(is_sorted(the_array) ? "sorted\n" : "not sorted\n");

    free(the_array.data);
    for (int i = 0; i < 4; i++) {
        free(bins[i].data);
    }

    exit(EXIT_SUCCESS);
}