#include "mt.h"
#define D(...) fprintf(new_stream, __VA_ARGS__)

int main(int argc, char **argv)
{
	int ssock, csock;
	struct sockaddr_in name;
	char buf[MAX_MSG_LENGTH] = {0};

	if(argc != 2) perro("args");

	csock = socket(AF_INET, SOCK_STREAM, 0);
	if(csock == -1) perro("socket");

	struct in_addr server_addr;
	if(!inet_aton(argv[1], &server_addr)) perro("inet_aton");

	ssock = socket(AF_INET, SOCK_STREAM, 0);
	if (ssock < 0) perro("opening socket");

	int optval = 1;
	setsockopt(ssock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	name.sin_family = AF_INET;
	name.sin_addr.s_addr = INADDR_ANY;
	name.sin_port = htons(CPORT);
	if(bind(ssock, (void*) &name, sizeof(name))) perro("binding tcp socket");
	if(listen(ssock, 1) == -1) perro("listen");
	
	struct sockaddr cli_addr;
	int cli_len = sizeof(cli_addr);
	int new_socket, new_fd, pid;
	FILE* new_stream;

	if(new_fd = dup(STDERR_FILENO) == -1) perro("dup");
	new_stream = fdopen(new_fd, "w");
	setbuf(new_stream, NULL); // sin buffering
	
	D("Initializing forward...\n");
	while(new_socket = accept(ssock, &cli_addr, &cli_len)) {
		D("Client connected.\nForking... ");
		if(pid = fork()) D("child pid = %d.\n", pid);
		else {
			pid = getpid();
			if(new_socket < 0) perro("accept");

	struct sockaddr_in connection;
	connection.sin_family = AF_INET;
	memcpy(&connection.sin_addr, &server_addr, sizeof(server_addr));

	connection.sin_port = htons(SPORT);
	if (connect(csock, (const struct sockaddr*) &connection, sizeof(connection)) != 0) perro("connect");
			

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
				if(send(csock, buf, strlen(buf)+1, 0) < 0) perro("send");
	filled = 0;	
	filled = recv(csock, buf, MAX_MSG_LENGTH-1, 0);
	
		buf[filled] = '\0';
		D("%s", buf);
		fflush(stdout);
				if(send(new_socket, buf, strlen(buf)+1, 0) < 0) perro("send");

				
				D("\t[%d] Finished executing command.\n", pid);
			}
			close(new_socket);
			D("\t[%d] Dying.", pid);
			exit(0);
		}
	}
	fclose(new_stream);
	close(ssock);
	return 0;
}