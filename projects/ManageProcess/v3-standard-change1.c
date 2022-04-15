/*
    该版本为标准答案修改成自己的数据结构后的答案。
*/
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
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 16

struct size_and_data
{
    int size;
    int *data;
};

struct bin_info
{
    int size;
    int *data;
};

struct fork_args
{
    struct size_and_data array; // which part of the array to deal with
    struct bin_info *bins;
    int status;
    pid_t cpid;
    int pipefd[2];
};

void print_data(struct size_and_data array)
{
    for (int i = 0; i < array.size; ++i)
        printf("%d ", array.data[i]);
    printf("\n");
}

/* Check to see if the data is sorted. */
bool is_sorted(struct size_and_data array)
{
    bool sorted = true;
    for (int i = 0; i < array.size - 1; i++)
    {
        if (array.data[i] > array.data[i + 1])
            sorted = false;
    }
    return sorted;
}

/* Fill the array with random data. */
void produce_random_data(struct size_and_data array)
{
    srand(1); // the same random data seed every time
    for (int i = 0; i < array.size; i++)
    {
        array.data[i] = rand() % 1000;
    }
}

/* Split the data into 4 bins. */
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

/* Allocate space for the data or a bin. */
int *allocate(int size)
{
    int *space;
    space = (int *)calloc(size, sizeof(int));
    if (space == NULL)
    {
        perror("Problem allocating memory.\n");
        exit(EXIT_FAILURE);
    }
    return space;
}

/* Move the data from the bins back to the original array. */
void move_back(struct size_and_data array, struct bin_info bins[])
{
    for (int bin = 0; bin < 4; bin++)
    {
        for (int i = 0; i < bins[bin].size; i++)
        {
            *array.data++ = bins[bin].data[i];
        }
    }
}

/* The slow insertion sort. */
void insertion(struct bin_info bin)
{
    for (int i = 1; i < bin.size; i++)
    {
        for (int j = i; j > 0; j--)
        {
            if (bin.data[j - 1] > bin.data[j])
            {
                int temp;
                temp = bin.data[j];
                bin.data[j] = bin.data[j - 1];
                bin.data[j - 1] = temp;
            }
            else
            {
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    struct size_and_data the_array;
    struct bin_info bins[4];
    struct fork_args args[4];
    pid_t	pidd[4];
    int fd[4][2];
    int status[4];

    if (argc < 2)
    {
        the_array.size = SIZE;
    }
    else
    {
        the_array.size = pow(2, atoi(argv[1]));
    }

    the_array.data = allocate(the_array.size);
    for (int i = 0; i < 4; i++)
    {
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
    for (int i = 0; i < 4; i++)
    {
        sum += bins[i].size;
    }
    printf("Total size of bins: %d\n", sum);

    for (int i = 0; i < 4; i++)
    {

        if (pipe(fd[i]) < 0)
        {
            perror("Problem creating pipe.\n");
            exit(EXIT_FAILURE);
        }

        pidd[i] = fork();

        //对啊，以下if套进一个for循环就没有....
        if (pidd[i] < 0)
        {
            perror("Problem creating fork.\n");
            exit(EXIT_FAILURE);
        }
        else if (pidd[i] == 0)
        {
            struct tms child_start_times, child_finish_times;
            time_t child_start_clock, child_finish_clock;

            child_start_clock = time(NULL);
            times(&child_start_times);
            printf("child[%d] start ctime in clock ticks: %ld\n", i, child_start_times.tms_utime);

            insertion(bins[i]);

            close(fd[i][0]);
            write(fd[i][1],bins[i].data,sizeof(int)*bins[i].size);
            close(fd[i][1]);

            times(&child_finish_times);
            child_finish_clock = time(NULL);
            printf("child[%d] finish time in clock ticks: %ld\n", i, child_finish_times.tms_utime);
            printf("child[%d] elapsed time in seconds: %ld\n", i, child_finish_clock - child_start_clock);

            _exit(EXIT_SUCCESS);//！学习
        }
        else
        {
            close(fd[i][1]);

            int pos = 0;
            int size = 0;
            while ((size = read(fd[i][0], bins[i].data + pos / sizeof(int), sizeof(int)*bins[i].size)) > 0)//测bug经验：觉得数据结构有问题才导致差异，换成自己的数据结构就好了
            {                                                                                                       //或者bins[i].data强行赋值
                printf("child [%d] pipe pos=%d size=%d\n", i, pos, size);
                pos += size;
            };

            close(fd[i][0]);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        waitpid(pidd[i], &status[i], 0);
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
    for (int i = 0; i < 4; i++)
    {
        free(bins[i].data);
    }

    exit(EXIT_SUCCESS);
}