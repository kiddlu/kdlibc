
#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <signal.h>

static pid_t child_pid = (pid_t)-1;

void fatal(const char *msg) {
    fprintf(stderr, "%s", msg);
    fprintf(stderr,  "%s", msg);
    exit(-1);
}

void usage() {
    fatal(
        "Usage: subprocess [-d] BINARY [ARGS ...]\n"
        "\n"
        "Forks and executes BINARY ARGS, redirecting stdout and stderr to\n"
        "the Android logging system. Tag is set to BINARY, priority is\n"
        "always stderr.\n"
        "\n"
        "-d: Causes subprocess to SIGSEGV when BINARY terminates\n"
        "    fault address is set to the status of wait()\n");
}

void forward_signal(int sigid) {
    fprintf(stderr,  "Got signal %d.\n", sigid);
    if (child_pid != (pid_t)-1) {
        fprintf(stderr,  "Forwards signal %d to %d.\n",
             sigid, child_pid);
        kill(child_pid, sigid);
    }
}


void post_process(void)
{
#if 0
#define REBOOT_WAIT_SECONDS (1)

	sleep(REBOOT_WAIT_SECONDS);

	fprintf(stderr,  "this is wrppaer post process, reboot\n");
	system("reboot");
#else
#endif
}

void parent(const char *tag, int seg_fault_on_exit, int parent_read) 
{
    int status;
    char buffer[4096];

    int a = 0;  // start index of unprocessed data
    int b = 0;  // end index of unprocessed data
    int sz;

    while ((sz = read(parent_read, &buffer[b], sizeof(buffer) - 1 - b)) > 0) {

        sz += b;
        // Log one line at a time
        for (b = 0; b < sz; b++) {
            if (buffer[b] == '\r') {
                buffer[b] = '\0';
            } else if (buffer[b] == '\n') {
                buffer[b] = '\0';
                fprintf(stderr, "%s\n", &buffer[a]);
                a = b + 1;
            }
        }

        if (a == 0 && b == sizeof(buffer) - 1) {
            // buffer is full, flush
            buffer[b] = '\0';
            fprintf(stderr, "%s\n", &buffer[a]);
            b = 0;
        } else if (a != b) {
            // Keep left-overs
            b -= a;
            memmove(buffer, &buffer[a], b);
            a = 0;
        } else {
            a = 0;
            b = 0;
        }

    }
    // Flush remaining data
    if (a != b) {
        buffer[b] = '\0';
        fprintf(stderr, "%s\n", &buffer[a]);
    }
    status = 0xAAAA;
    if (wait(&status) != -1) {  // Wait for child
        if (WIFEXITED(status) && WEXITSTATUS(status)) {
            fprintf(stderr,  "%s terminated by exit(%d)", tag,
                    WEXITSTATUS(status));
            if (!seg_fault_on_exit) {
				post_process();
                exit(WEXITSTATUS(status));
            }
        } else if (WIFSIGNALED(status))
            fprintf(stderr,  "%s terminated by signal %d", tag,
                    WTERMSIG(status));
        else if (WIFSTOPPED(status))
            fprintf(stderr,  "%s stopped by signal %d", tag,
                    WSTOPSIG(status));
    } else
        fprintf(stderr,  "%s wait() failed: %s (%d)", tag,
                strerror(errno), errno);
    if (seg_fault_on_exit)
        *(int *)status = 0;  // causes SIGSEGV with fault_address = status

	post_process();
}

void child(int argc, char* argv[]) {
    // create null terminated argv_child array
    char* argv_child[argc + 1];
    memcpy(argv_child, argv, argc * sizeof(char *));
    argv_child[argc] = NULL;

    if (execvp(argv_child[0], argv_child)) {
        fprintf(stderr, 
            "executing %s failed: %s\n", argv_child[0], strerror(errno));
        exit(-1);
    }
}

int main(int argc, char* argv[]) {
    pid_t pid;
    int seg_fault_on_exit = 0;

    int parent_ptty;
    int child_ptty;
    char *child_devname = NULL;

    if (argc < 2) {
        usage();
    }
    if (strncmp(argv[1], "-d", 2) == 0) {
        seg_fault_on_exit = 1;
        argc--;
        argv++;
    }

    if (argc < 2) {
        usage();
    }

    /* Use ptty instead of socketpair so that STDOUT is not buffered */
    parent_ptty = open("/dev/ptmx", O_RDWR);
    if (parent_ptty < 0) {
        fatal("Cannot create parent ptty\n");
    }

    if (grantpt(parent_ptty) || unlockpt(parent_ptty) ||
            ((child_devname = (char*)ptsname(parent_ptty)) == 0)) {
        fatal("Problem with /dev/ptmx\n");
    }
    pid = fork();
    if (pid < 0) {
        fatal("Failed to fork\n");
    } else if (pid == 0) {
        child_ptty = open(child_devname, O_RDWR);
        if (child_ptty < 0) {
			printf("can't open child dev:%s\n", child_devname);
            fatal("Problem with child ptty\n");
        }

        // redirect stdout and stderr
        close(parent_ptty);
        dup2(child_ptty, 1);
        dup2(child_ptty, 2);
        close(child_ptty);

        child(argc - 1, &argv[1]);

    } else {
        // switch user and group to "log"
        // this may fail if we are not root, 
        // but in that case switching user/group is unnecessary 
        //int ret = setgid(AID_LOG);
		// linux /dev/log_xxx node mode is only root can operation, so set uid to root
        //if (setuid(0) == -1) {
          // set sighandler to forward signals to child process only when
          // setuid fails and/so uid of this parent process is same to uid of
          // child process.
          child_pid = pid;
          signal(SIGHUP, forward_signal);
          signal(SIGTERM, forward_signal);
          signal(SIGINT, forward_signal);
		  //perror("setuid failed");
        //}

        parent(argv[1], seg_fault_on_exit, parent_ptty);
    }

    return 0;
}
