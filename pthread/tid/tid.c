#include <stdio.h>
#include <sys/syscall.h>//Linux system call for thread id
#include <assert.h>
#include <pthread.h>

void *nbi(void *arg)
{
        int i;
        printf(child thread lwpid %u\n, syscall(SYS_gettid));
        printf(child thread tid %u\n, pthread_self());
        scanf(%d, i);//code dump
}
int main()
{
        pthread_t tid;
        int rc;
        printf(main thread lwpid %u\n, syscall(SYS_gettid));
        printf(main thread tid %u\n, pthread_self());
        rc = pthread_create(&amp;tid, NULL, nbi, NULL);
        assert(rc);
        pthread_join(tid, NULL);

        return 0;
}