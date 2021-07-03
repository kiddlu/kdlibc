#include "mt.h"

#define D(...) fprintf(new_stream, __VA_ARGS__)


//=============================================================
/* 
 *  popen.c     cp from W. Richard Stevens 
 */  
  
#include    <sys/wait.h>   
#include    <errno.h>   
#include    <fcntl.h>   
#include    <sys/resource.h>
static pid_t    *childpid = NULL;  /* ptr to array allocated at run-time */  
static int      maxfd;  /* from our open_max(), {Prog openmax} */  

FILE *my_popen_timeout(const char *cmdstring, const char *type, const char *to)  
{  
    int     i, pfd[2];  
    pid_t   pid;  
    FILE    *fp;  
  
            /* only allow "r" or "w" */  
    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {  
        errno = EINVAL;     /* required by POSIX.2 */  
        return(NULL);  
    }  
  
    if (childpid == NULL) {     /* first time through */  
                /* allocate zeroed out array for child pids */  

        /* OPEN_MAX is canceled after linux-2.6.24, and become a part of RLIMIT_NOFILE. 
        Execute shell 'ulimit -a' to get more information */
        struct rlimit limit;
        if(getrlimit(RLIMIT_NOFILE, &limit) == -1)
        {
            perror("getrlimit fail");
            return(NULL);
        }
        maxfd = limit.rlim_cur;
        //print("rlimit = %d.\n", maxfd);

        if ( (childpid = calloc(maxfd, sizeof(pid_t))) == NULL)  
            return(NULL);  
    }  
  
    if (pipe(pfd) < 0)  
        return(NULL);   /* errno set by pipe() */  
  
    if ( (pid = fork()) < 0)  
        return(NULL);   /* errno set by fork() */  
    else if (pid == 0) {                            /* child */  
        if (*type == 'r') {  
            close(pfd[0]);  
            if (pfd[1] != STDOUT_FILENO) {  
                dup2(pfd[1], STDOUT_FILENO); 
				dup2(pfd[1], STDERR_FILENO); 				
                close(pfd[1]);  
            }  
        } else {  
            close(pfd[1]);  
            if (pfd[0] != STDIN_FILENO) {  
                dup2(pfd[0], STDIN_FILENO);  
                close(pfd[0]);  
            }  
        }  
            /* close all descriptors in childpid[] */  
        for (i = 0; i < maxfd; i++)  
            if (childpid[ i ] > 0)  
                close(i);  
  
        //execlp("/bin/sh", "sh", "-c", cmdstring, (char *) 0);
		execlp("timeout","timeout", to, "sh","-c" ,cmdstring, (char *) 0);
        _exit(127);  
    }  
                                /* parent */  
    if (*type == 'r') {  
        close(pfd[1]);  
        if ( (fp = fdopen(pfd[0], type)) == NULL)  
            return(NULL);  
    } else {  
        close(pfd[0]);  
        if ( (fp = fdopen(pfd[1], type)) == NULL)  
            return(NULL);  
    }  
    childpid[fileno(fp)] = pid; /* remember child pid for this fd */  
    return(fp);  
}  
  
int my_pclose(FILE *fp)  
{  
  
    int     fd, stat;  
    pid_t   pid;  
  
    if (childpid == NULL)  
        return(-1);     /* popen() has never been called */  
  
    fd = fileno(fp);  
    if ( (pid = childpid[fd]) == 0)  
        return(-1);     /* fp wasn't opened by popen() */  
  
    childpid[fd] = 0;  
    if (fclose(fp) == EOF)  
        return(-1);  
  
    while (waitpid(pid, &stat, 0) < 0)  
        if (errno != EINTR)  
            return(-1); /* error other than EINTR from waitpid() */  
  
    return(stat);   /* return child's termination status */  
}

/*============================================================================*/
#include <ctype.h>
#define ARG_MAX_COUNT    1024      /* max number of arguments to a command */
/* Type declarations */
struct command {		   /* a struct that stores a parsed command */
	int argc;                  /* number of arguments in the comand */
	char *name;                /* name of the command */
	char *argv[ARG_MAX_COUNT]; /* the arguments themselves */
};

struct commands {                  /* struct to store a command pipeline */
	int cmd_count;             /* number of commands in the pipeline */
	struct command *cmds[];    /* the commands themselves */
};

int is_blank(char *input)
{
	int n = (int) strlen(input);
	int i;

	for (i = 0; i < n; i++) {
		if (!isspace(input[i]))
			return 0;
	}
	return 1;
}

int is_multi_cmd(char *input)
{
	int n = (int) strlen(input);
	int i;

	for (i = 0; i < n; i++) {
		if ((input[i]) == ';')
			return 1;
	}
	return 0;
}

/* Parses a single command into a command struct.
 * Allocates memory for keeping the struct and the caller is responsible
 * for freeing up the memory
 */
struct command *parse_command(char *input)
{
	int tokenCount = 0;
	char *token;

	/* allocate memory for the cmd structure */
	struct command *cmd = calloc(sizeof(struct command) +
				     ARG_MAX_COUNT * sizeof(char *), 1);

	if (cmd == NULL) {
		fprintf(stderr, "error: memory alloc error\n");
		exit(EXIT_FAILURE);
	}

	/* get token by splitting on whitespace */
	token = strtok(input, " ");

	while (token != NULL && tokenCount < ARG_MAX_COUNT) {
		cmd->argv[tokenCount++] = token;
		token = strtok(NULL, " ");
	}
	cmd->name = cmd->argv[0];
	cmd->argc = tokenCount;
	return cmd;
}


/* Parses a command with pipes into a commands* structure.
 * Allocates memory for keeping the struct and the caller is responsible
 * for freeing up the memory
 */
struct commands *parse_commands_with_pipes(char *input)
{
	int commandCount = 0;
	int i = 0;
	char *token;
	char *saveptr;
	char *c = input;
	struct commands *cmds;

	while (*c != '\0') {
		if (*c == '|')
			commandCount++;
		c++;
	}

	commandCount++;

	cmds = calloc(sizeof(struct commands) +
		      commandCount * sizeof(struct command *), 1);

	if (cmds == NULL) {
		fprintf(stderr, "error: memory alloc error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok_r(input, "|", &saveptr);
	while (token != NULL && i < commandCount) {
		cmds->cmds[i++] = parse_command(token);
		token = strtok_r(NULL, "|", &saveptr);
	}

	cmds->cmd_count = commandCount;
	return cmds;
}

/* Returns whether a command is a built-in. As of now
 * one of [exit, cd, history]
 */
int check_built_in(struct command *cmd)
{
	return strcmp(cmd->name, "exit") == 0 ||
		strcmp(cmd->name, "cd") == 0;
}

//return 0,not build-in
//return 1, run build-in ok
//return -1,run build-in err
//build-in cmd must be very single
int system_input_handle(char *cmd)
{
	int ret;
	
	char *input = malloc(strlen(cmd));
	memcpy(input, cmd, strlen(cmd));

	if (strlen(input) > 0 && !is_blank(input) && !is_multi_cmd(input) && input[0] != '|') {
		char *linecopy = strdup(input);

		struct commands *commands =
			parse_commands_with_pipes(input);

		for(int i=0;i<commands->cmd_count;i++) {
			char *name = commands->cmds[i]->name;
			int argc = commands->cmds[i]->argc;
			char **argv = commands->cmds[i]->argv;
			//printf("cmd[%d]: %s", i, name);
			for(int j=0;j<argc;j++) {
				//printf(" argv:%d %s size:%d", j, argv[j],strlen(argv[j]));
			}
			//printf("\n");
		}

		if (commands->cmd_count > 1) {
			return 0;
		} else if(check_built_in(commands->cmds[0])) {
			//now we handle it
			struct command *cmd = commands->cmds[0];

			if (strcmp(cmd->name, "exit") == 0){
				printf("exit?\n");
				return 1;
			}
			
			if (strcmp(cmd->name, "cd") == 0) {

				int cmd_len = strlen(cmd->argv[1]);
				if(cmd->argv[1][cmd_len - 1] == '\n'){
					cmd->argv[1][cmd_len - 1] = 0;
				}
				ret = chdir(cmd->argv[1]);
				if (ret != 0) {
					fprintf(stderr, "error: unable to change dir\n");
					return -1;
				}
				return 1;
			}

		}
		free(linecopy);
	}
	free(input);
}
/*============================================================================*/

int my_system(const char * cmd, int socket) 
{
	FILE * fp;
	int res; char buf[1024];
	int nr;
	char *timeout = "3";
	int total_len = 0;

	if (cmd == NULL){
		printf("my_system cmd is NULL!\n");
		return -1;
	}
	
	if((fp = my_popen_timeout(cmd, "r", timeout)) == NULL){
		perror("popen");
		printf("popen error: %s/n", strerror(errno));
		return -1;
	} else {
       for(;;) {
           nr = fread(buf, 1, sizeof(buf)-1, fp);
           if(nr <= 0) {
               break;
           }
		   total_len += nr;
		   send(socket, buf, strlen(buf), 0);
		   memset(buf, 0x0, strlen(buf));
       }
	   my_pclose(fp);
	}
	
	printf("total_len is %d\n", total_len);

	return 0;
}

#define MAX_MEM_SIZE (64*1024)

int my_system_memout(char * cmd, char *memout, int *ret_len) 
{
	FILE * fp;
	int res;
	int nr;
	char *timeout = "3";
	int total_len = 0;

	memset(memout, 0x0, strlen(memout));

	if (cmd == NULL){
		printf("my_system cmd is NULL!\n");
		return -1;
	}

	res = system_input_handle(cmd);
	if (res > 0) {
		printf("run build-in cmd OK\n");
		return 1;
	} else if (res < 0) {
		printf("run build-in cmd Err, continue\n");
	}	

	if((fp = my_popen_timeout(cmd, "r", timeout)) == NULL){
		perror("popen");
		printf("popen error: %s/n", strerror(errno));
		return -1;
	} else {
       for(;;) {
           nr = fread(memout, 1, MAX_MEM_SIZE-1, fp);
           if(nr <= 0) {
               break;
           }
		   total_len += nr;
       }
	   my_pclose(fp);
	}
	
	//printf("total_len is %d\n", total_len);
	*ret_len = total_len;
	return 0;
}

//=============================================================


int main() {
	int sock;
	struct sockaddr_in name;
	char buf[MAX_MSG_LENGTH] = {0};
	
	char memout[MAX_MEM_SIZE];
	int len;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) perro("opening socket");

	int optval = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	name.sin_family = AF_INET;
	name.sin_addr.s_addr = INADDR_ANY;
	name.sin_port = htons(SPORT);
	if(bind(sock, (void*) &name, sizeof(name))) perro("binding tcp socket");
	if(listen(sock, 1) == -1) perro("listen");
	
	struct sockaddr cli_addr;
	int cli_len = sizeof(cli_addr);
	int new_socket, new_fd, pid;
	FILE* new_stream;
	
	if(new_fd = dup(STDERR_FILENO) == -1) perro("dup");
	new_stream = fdopen(new_fd, "w");
	setbuf(new_stream, NULL); // sin buffering
	
	D("Initializing server...\n");
	while(new_socket = accept(sock, &cli_addr, &cli_len)) {
		D("Client connected.\nForking... ");
		if(pid = fork()) D("child pid = %d.\n", pid);
		else {
			pid = getpid();
			if(new_socket < 0) perro("accept");
			//if(dup2(new_socket, STDOUT_FILENO) == -1) perro("dup2");
			//if(dup2(new_socket, STDERR_FILENO) == -1) perro("dup2");
			while(1) {
				int readc = 0, filled = 0;
				while(1) {
					readc = recv(new_socket, buf+filled, MAX_MSG_LENGTH-filled-1, 0);
					if(!readc) break;
					filled += readc;
					if(buf[filled-1] == '\0') break;
				}
				if(!readc) {
					D("\t[%d] Client disconnected.\n", pid);
					break;
				}
				D("\t[%d] Command received: %s", pid, buf);
				//system(buf);
				//my_system(buf, new_socket);
				my_system_memout(buf,memout, &len);
				send(new_socket, memout, len, 0);
				D("\t[%d] Finished executing command.\n", pid);
				send(new_socket, "> ", 3, MSG_NOSIGNAL);
			}
			close(new_socket);
			D("\t[%d] Dying.", pid);
			exit(0);
		}
	}
	fclose(new_stream);
	close(sock);
	return 0;
}