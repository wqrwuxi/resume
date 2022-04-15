/*
    A program which splits data using threads for Operating Systems
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

struct thread_args
{
    struct size_and_data array; // which part of the array to deal with
    struct bin_info *bins;
};

pthread_mutex_t lock;

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

/* Split some data into a bin. */
void *split_data(void *params)
{
    struct thread_args args = *(struct thread_args *)params;
    struct size_and_data array = args.array;
    struct bin_info *bins = args.bins;

    pthread_mutex_lock(&lock);

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

    pthread_mutex_unlock(&lock);
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
    struct thread_args args[4];
    pthread_t bin_threads[4];

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

    int *start_of_data = the_array.data;

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < 4; i++)
    {
        args[i].array.size = the_array.size / 4;
        args[i].array.data = start_of_data;
        start_of_data += the_array.size / 4; // moving down to the next quarter of the array
        args[i].bins = bins;                 // these are pointers
        if (pthread_create(&bin_threads[i], NULL, split_data, (void *)&args[i]))
        {
            perror("Problem creating thread.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        pthread_join(bin_threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    int sum = 0;
    for (int i = 0; i < 4; i++)
    {
        sum += bins[i].size;
    }
    printf("Total size of bins: %d\n", sum);

    for (int i = 0; i < 4; i++)
    {
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
    for (int i = 0; i < 4; i++)
    {
        free(bins[i].data);
    }

    exit(EXIT_SUCCESS);
}