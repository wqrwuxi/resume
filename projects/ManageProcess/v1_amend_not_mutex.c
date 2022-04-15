/* 案例1 */
//不用锁的自己版本。pthread_create使用指针以进行地址传递
//具体点是pthread_create第四个参数是值传递，但是传递的结构中数组以指针形式出现，传进去的是数组地址，不是数组本身
//不用锁的思路：把bins[i]拆成4小个，给每个进程，最后再合并成一个bins[i]
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

#define SIZE    16

 struct size_and_data{
    int size;
    int *data;
};

struct bin_info{
    int size;
    int *data;
};

struct  thread_args{
    struct size_and_data array; // 哪一部分的数据要处理
    struct bin_info* bins;//想在pthread_create的函数中传递参数并修改，必须用指针，这样用了值传递
};

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

/* 拆分数据到某个 bins*/
void *split_data(void *params) {
    struct thread_args args = *(struct thread_args *)params;
   struct size_and_data array = args.array;
   struct bin_info *bins = args.bins;
    
    for(int j=0;j<4;j++)
        {
            printf("bins_in:%d\n",bins[j].size);
        }

    for (int i = 0; i < array.size; i++) {
        int number = array.data[i];
        if (number < 250) {
            bins[0].data[bins[0].size++] = number;
        } else if (number < 500) {
            bins[1].data[bins[1].size++] = number;
        } else if (number < 750) {
            args.bins[2].data[bins[2].size++] = number;
        } else {
            bins[3].data[bins[3].size++] = number;
        }
    }

    for(int j=0;j<4;j++)
        {
            printf("bins_out:%d\n",bins[j].size);
        }

}

/* 移动bins里面的数据到原始数组 */
void move_back_bin(struct bin_info bins[], struct thread_args *args) {
    for(int i=0;i<4;i++)
    {
        printf("args->bins[i].size:%d,%d\n",i,args->bins[i].size);
        for(int j=0;j<args->bins[i].size;j++)
        {
            bins[i].data[bins[i].size++] = args->bins[i].data[j];
        }
        printf("bins[i].size:%d,%d\n",i,bins[i].size);

    }

}

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
}

/* 主函数 */
int main(int argc, char *argv[]) {
    struct  size_and_data the_array;
    struct  bin_info bins[4];
    struct  thread_args args[4];
    pthread_t bin_threads[4];

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
    

    //main函数内初始化，不然怎么全局用
    //新加
    for(int i = 0; i < 4; i++)
    {   
        args[i].bins=(struct bin_info*)malloc(sizeof(struct bin_info)*4);
        for(int m=0;m<4;m++)
        {
          args[i].bins[m].size=0;
          args[i].bins[m].data=allocate(the_array.size/4);
        }
    }

    produce_random_data(the_array);

    if (the_array.size < 1025)
        print_data(the_array);

    struct tms start_times, finish_times;
    time_t start_clock, finish_clock;

    start_clock = time(NULL);
    times(&start_times);
    printf("start time in clock ticks: %ld\n", start_times.tms_utime);

    int *start_of_data = the_array.data;
    
    for (int i = 0; i < 4; i++) {

       // printf("bins-in%d:%d\n",i,bins->size); bin[0]的size，能指过去也是没相到的

        args[i].array.size = the_array.size / 4;
        args[i].array.data = start_of_data;
        start_of_data += the_array.size / 4;

        //4个线程是并发的，数据小的时候是依次算的，数据大了就团在一起了，一个没出另一个就进了，同一位置会被复写，所以数据变小了
        if (pthread_create(&bin_threads[i], NULL, split_data, (void *)&args[i])) {
            perror("Problem creating thread.\n");
            exit(EXIT_FAILURE);
        }


    }

    for (int i = 0; i < 4; i++) {
        
        pthread_join(bin_threads[i], NULL);
    }

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
           printf("args[i].bins[j].size:%d,%d,%d\n",i,j,args[i].bins[j].size);
        } 
    }

    for (int i = 0; i < 4; i++) 
    {
        move_back_bin(bins,&args[i]);
    }

    int sum = 0;
    for (int i = 0; i < 4; i++) {
        printf("bin zise:%d\n",bins[i].size);
        sum += bins[i].size;
    }
    printf("Total size of bins: %d\n", sum);

    for (int i = 0; i < 4; i++) {
        insertion(bins[i]);
    }

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
