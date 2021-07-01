#include "mt.h"

#define D(...) fprintf(new_stream, __VA_ARGS__)

int my_system(const char * cmd, int socket) 
{
	FILE * fp;
	int res; char buf[1024];
	int nr;
	
	int total_len = 0;

	if (cmd == NULL){
		printf("my_system cmd is NULL!\n");
		return -1;
	}
	if((fp = popen(cmd, "r")) == NULL){
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
       }
	   pclose(fp);
	}
	
	printf("total_len is %d\n", total_len);

	return 0;
}


int main() {
	int sock;
	struct sockaddr_in name;
	char buf[MAX_MSG_LENGTH] = {0};

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
				my_system(buf, new_socket);
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