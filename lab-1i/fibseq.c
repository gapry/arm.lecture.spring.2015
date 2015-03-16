#include <stdio.h>
#include <time.h>

extern int fibonacci(int x);

struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

void reset(struct timespec* start, struct timespec* end)
{
    (*start).tv_sec = 0;
    (*end).tv_nsec = 0;
}

int main(int argc, char **argv)
{
    int number=0;
    int result=0;
    struct timespec time_start, time_end;
   
    scanf("%d",&number);
  
    reset (&time_start, &time_end);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_start); 
    result = fibonacci(number);  
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_end); 
    if(result != -1) {
        printf("Time is %lu(s) : %lu(ns)\n", diff (time_start, time_end).tv_sec, diff (time_start, time_end).tv_nsec);
        printf("The fibonacci sequence at %d is: %d\n", number, result);
    } else {
        printf("It occurs overflow.\n");
    }
    reset (&time_start, &time_end);
}

