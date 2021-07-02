
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
 
static int loop;
static struct itimerval value;

void signalHandler(int signo)
{
    switch (signo){
        case SIGALRM:
            printf("Caught the SIGALRM signal!\n");
			loop++;
			if(loop == 5)
			{
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &value, NULL);				
			}
        break;
   }
}
 
int main(int argc, char *argv[])
{
    signal(SIGALRM, signalHandler);
 
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 5;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &value, NULL);
    
    for(;;);
     
    return 0;
}