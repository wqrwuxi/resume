/* 案例1 */
//难点1.1：4个fork，5个子进程，其中一个与主进程有关，不可以return，否则pid>0的大括号之后的内容全不能运行，其他4个要return
//(接上)4个fork，6个fork进程,fork前和pid>0的大括号之后的内容为一个进程号，且该号码为所有进程号中最小，自己叫它：main的进程
//(接上)剩下5个父进程与5个子进程对应
//(接上)5个父进程要与main的进程通信才能传数据
//难点1.2:waitpid的安排：4个子进程先waitpid再return，一个与主进程有关的子进程单独waitpid，不return
//难点2：write/read/close/open：比如bins[i].data，传这个地址进去，也可以运行
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
void split_data(struct thread_args args) {
    struct size_and_data array = args.array;
    struct bin_info *bins = args.bins;//这是个指针啊，传进来的不过是地址！
    pthread_mutex_lock(&mutex);    
    for (int i = 0; i < array.size; i++) {
        int number = array.data[i];
        if (number < 250) {
            bins[0].data[bins[0].size++] = number;
        } else if (number < 500) {
            bins[1].data[bins[1].size++] = number;
        } else if (number < 750) {
            bins[2].data[bins[2].size++] = number;
        } else {
            bins[3].data[bins[3].size++] = number;
        }
    }
    pthread_mutex_unlock(&mutex);
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
    int status;
    
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

    int *start_of_data = the_array.data;//the_array.data是个指针
    for (int i = 0; i < 4; i++) {
        args[i].array.size = the_array.size / 4;
        args[i].array.data = start_of_data;
        start_of_data += the_array.size / 4;// moving down to the next quarter of the array
        args[i].bins = bins;                // these are pointers
        split_data(args[i]);
    }

    int sum = 0;
    for (int i = 0; i < 4; i++) {
        sum += bins[i].size;
    }
    printf("Total size of bins: %d\n", sum);
    
    int m;

    int test=getpid();
    printf("main_pid %d\n",test);

    for(m=0;m<4;m++)
    {   
        if(pipe(fd[m])<0)
        {
            printf("Error in pipe %d: %s\n",m,(strerror(errno)));
        }

        if(pipe(fd_main[m])<0)
        {
            printf("Error in pipe main%d: %s\n",m,(strerror(errno)));
        }
        
        pid = fork();
        if(pid == 0 || pid == -1)
        {
            break;
        }
    }
    int test11=getpid();
    printf("after fork %d\n",test11);
        
    if (pid  < 0) 
    {
		perror("fork error\n");
        exit(1);
	} 
    else if(pid==0)
    {
        printf("child_pid %d\n",pidd[m]);
            printf("m: %d\n",m);
        
        if(m<4){

            struct tms fork_start_times, fork_finish_times;
            time_t fork_start_clock, fork_finish_clock;

            close(fd[m][0]);
            pidd[m]=getpid();

           

            fork_start_clock = time(NULL);
            times(&fork_start_times);
            printf("fork%d start time in clock ticks: %ld\n",m,fork_start_times.tms_utime);

            insertion(bins[m]);

            /*for(int j=0;j<bins[m].size;j++)
            {
                printf("%d ",bins[m].data[j]);
            }
            printf("after insert\n");*/

            if(write(fd[m][1],bins[m].data,sizeof(int)*bins[m].size)!=(sizeof(int)*bins[m].size))  //sizeof(bins[i].data)是指针大小,错
            {
                printf("Error in pipe write %d: %s\n",m,(strerror(errno)));
            }

            close(fd[m][1]);

            times(&fork_finish_times);
            fork_finish_clock = time(NULL);
            printf("fork%d finish time in clock ticks: %ld\n",m,fork_finish_times.tms_utime);
            printf("fork%d Total elapsed time in seconds: %ld\n",m,(fork_finish_clock - fork_start_clock));

        }//if(m<4)
        return 0;

    }
    else if(pid>0)
    {   

       int test2=getpid();
        printf("parent_pid %d:%d:%d\n",m,test2,pid);
            
        if(m<4)
        {
            close(fd[m][1]);

            if (read(fd[m][0],bins[m].data,sizeof(int)*bins[m].size)!=(sizeof(int)*bins[m].size))//&bins[i]错，传结构无法赋值。bins[i].data[j]可以。当前为数组也可以
            {                                                                                    //sizeof(int)*bins[i].size外的括号少了会不认
                printf("Error in pipe read %d: %s\n",m,(strerror(errno)));
            }

            close(fd[m][0]);

            close(fd_main[m][0]);

            if(write(fd_main[m][1],bins[m].data,sizeof(int)*bins[m].size)!=(sizeof(int)*bins[m].size))  //sizeof(bins[i].data)是指针大小,错
            {
                printf("Error in pipe write %d: %s\n",m,(strerror(errno)));
            }

            close(fd_main[m][1]);
                
            do 
            {   
                // WNOHANG 非阻塞 轮询 等待带子进程结束
                child_pid = waitpid(pid, NULL, WNOHANG); 
                sleep(1); 
            }while(child_pid == 0);

            return 0;
        }//return 0;//这个！查了很久很久！fork()会产生一个新的父进程是为什么？？

        do 
        {   
            // WNOHANG 非阻塞 轮询 等待带子进程结束
            child_pid = waitpid(pid, NULL, WNOHANG); 
            sleep(1); 
        }while(child_pid == 0);
    }
    
    for(int i=0;i<4;i++)
    {
        close(fd_main[i][1]);

        if (read(fd_main[i][0],bins[i].data,sizeof(int)*bins[i].size)!=(sizeof(int)*bins[i].size))//&bins[i]错，传结构无法赋值。bins[i].data[j]可以。当前为数组也可以
        {                                                                                    //sizeof(int)*bins[i].size外的括号少了会不认
            printf("Error in pipe read %d: %s\n",i,(strerror(errno)));
        }

        close(fd_main[i][0]);
    }

    move_back(the_array, bins);

    times(&finish_times);
    finish_clock = time(NULL);
    printf("finish time in clock ticks: %ld\n", finish_times.tms_utime);
    printf("Total elapsed time in seconds: %ld\n", finish_clock - start_clock);

    if (the_array.size < 1025)
        print_data(the_array);

    printf(is_sorted(the_array) ? "sorted\n" : "not sorted\n");

    /*int test1=getpid();
    printf("main_pid-after %d\n",test1);*/

    free(the_array.data);
    for (int i = 0; i < 4; i++) {
        free(bins[i].data);
    }

    exit(EXIT_SUCCESS);
}