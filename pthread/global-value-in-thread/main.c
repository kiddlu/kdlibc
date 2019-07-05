#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
 
int global = 0;
 
void *assign_value(void *param)
{
    printf("2 is %d\n", global);
    fflush(stdout);

    global = 3;

    printf("3 is %d\n", global);
    fflush(stdout);
}
 
int main()
{
    pthread_t thread;

    global = 5;
    printf("1 is %d\n", global);
    fflush(stdout);

    pthread_create(&thread, NULL, assign_value, (void *)NULL);

    printf("4 is %d\n", global);
    fflush(stdout);

    sleep(1);
    printf("5 is %d\n", global);
    fflush(stdout);

    pthread_join(thread, NULL);

    printf("6 is %d\n", global);
    fflush(stdout);

    return 0;
}
