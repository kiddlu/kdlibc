
/******************************************************************
* pseudo-telnetd util ;)
* compile: 
*
* linux: gcc -Wall -O2 logind.c -o logind -lutil
* osx: gcc -Wall -O2 logind.c -o logind
* solaris: gcc -Wall -O2 logind.c -o logind -lnsl -lsocket
*
*******************************************************************/

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <errno.h>
#include <utmp.h>
#include <sys/select.h>
#include <stdlib.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#if defined(__gnu_linux__) 
#include <pty.h>
#endif 

#if defined(__APPLE__)
#include <util.h>
#endif



/*********************************
* macros aux, config
**********************************/

#define SHELL "/bin/sh"
#define PORT 8818
#define IP "127.0.0.1"
#define WORK_DIR "/tmp"
#define SZ_SBUF 512
#define LOG_FILE "/tmp/logind.log"

#define TRY(x, msg) if(x == -1) x_abort(msg " err")
#define LOG(x) write(f_log, x, strlen(x))



#if 1
#define dbg_printf printf
#else 
#define dbg_printf {;}
#endif

int my_login_tty(int fd)
{
	(void) setsid();
#ifdef TIOCSCTTY
	if (ioctl(fd, TIOCSCTTY, (char *)NULL) == -1)
		return (-1);
#else
	{
	  /* This might work.  */
	  char *fdname = ttyname (fd);
	  int newfd;
	  if (fdname)
	    {
	      if (fd != 0)
		(void) close (0);
	      if (fd != 1)
		(void) close (1);
	      if (fd != 2)
		(void) close (2);
	      newfd = open (fdname, O_RDWR);
	      (void) close (newfd);
	    }
	}
#endif
	(void) dup2(fd, 0);
	(void) dup2(fd, 1);
	(void) dup2(fd, 2);
	if (fd > 2)
		(void) close(fd);
	return (0);
}
int my_openpty (int *amaster, int *aslave, char *name, struct termios *termp,
	 struct winsize *winp)
{


#ifdef PATH_MAX
  char buf[PATH_MAX];
#else
  char buf[512];
#endif


  int master, slave;

  master = posix_openpt (O_RDWR);
  if (master == -1)
    return -1;

  if (grantpt (master))
    goto fail;

  if (unlockpt (master))
    goto fail;


  if (ptsname_r(master, buf, sizeof buf))
    goto fail;

  slave = open (buf, O_RDWR | O_NOCTTY);
  if (slave == -1)
    {
      goto fail;
    }

  if(termp)
    tcsetattr (slave, TCSAFLUSH, termp);
  if (winp)
    ioctl (slave, TIOCSWINSZ, winp);

  *amaster = master;
  *aslave = slave;
  if (name != NULL)
    strcpy (name, buf);

  return 0;

 fail:
  close (master);
  return -1;
}

void dumphex(void *data, uint32_t size)
{
	char ascii[17];
	unsigned int i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		dbg_printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			dbg_printf(" ");
			if ((i+1) % 16 == 0) {
				dbg_printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					dbg_printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					dbg_printf("   ");
				}
				dbg_printf("|  %s \n", ascii);
			}
		}
	}
}

/*********************************
* variables globales
**********************************/

int pid, term_m, term_s, sock_c, sock_s, f_log;


/*********************************
* funciones 
**********************************/

// quitamos retornos 
void chomp(char *s)
{
	while (*s) if (*s == '\n' || *s == 0x0d) *s = 0;
		else s++; 
}

// salida limpia 
void clean()
{
	close(term_m);
	close(term_s);
	close(sock_c);
	close(sock_s);
	close(f_log);
}


// abort con msg
void x_abort(const char *msg)
{
	perror(msg);
	clean();
	exit(EXIT_FAILURE);
}


// manejador de interrupciones
void sig(int intr)
{
	printf("exit code: %d\n", intr);

	// segun int
	switch(intr) 
	{
		case SIGTERM:
		case SIGINT:
		case SIGQUIT:
		case SIGCHLD:

			// salida
			printf("logout!\n");

		break;
	}

	// salida limpia
	kill(pid, SIGTERM);
	clean();
	exit(0);
}


// funcion principal
int main()
{
	struct winsize win;
	struct termios tt;
	char *const args[] = { SHELL, "-i", 0 };
	fd_set  rfd;
	int r, sz, on = 1;
	static char sbuf[SZ_SBUF];
	struct sockaddr_in ads, adc;
	socklen_t len;

	// parametros terminal
	tcgetattr(0, &tt);
	ioctl(0, TIOCGWINSZ, &win);

	// abrimos fich log
	TRY((f_log = open(LOG_FILE, O_WRONLY | 
		O_CREAT | O_APPEND, 0644)), "open");

	LOG("logind started!\n");

	//
	// primer fork (daemon)
	// 
#if 0
	LOG("fork 1!\n");
	TRY((pid = fork()), "fork"); 

	// padre? salimos
	if (pid) return 0;


	//
	// a partir de aqui, hijo
	//

	// creamos una nueva sesion
	LOG("setsid!\n");
	TRY(setsid(), "setsid");

	// directorio de trabajo
	LOG("chdir!\n");
	TRY(chdir(WORK_DIR), "chdir");
#endif

	// creamos pseudo-terminales
	LOG("openpty!\n");
	TRY(my_openpty(&term_m, &term_s, NULL, &tt, &win), "opentty");

	// instalamos manejador ints 
	LOG("set signals!\n");
	signal(SIGTERM, sig);
	signal(SIGINT, sig);
	signal(SIGQUIT, sig);
	signal(SIGCHLD, sig);

	// tercer fork 
	LOG("fork 2!\n");
	TRY((pid = fork()), "fork");

	// hijo?
	if (pid == 0) //Child
	{
		// cerramos term_m
		close(term_m);

		// preparamos tty
		//LOG("login_tty!\n");
		TRY(my_login_tty(term_s), "login_tty");
		// desactivamos echo
		LOG("tcsetattr!\n");
		tt.c_lflag &= ~ECHO;
		tcsetattr(0, TCSANOW, &tt);

		// activamos echo 
		//tt.c_lflag |= ECHO;
		//tcsetattr(0, TCSANOW, &tt);

		// lanzamos shell
		LOG("execv!\n");
		TRY(execv(SHELL, args), "execv");

		// llegados aqui, algo mal?
		LOG("execv error!\n");
		clean();
		kill(0, SIGTERM);
		exit(0);
	}

	// 
	// a partir de aqui, padre 
	//

	// abrimos socket 
	LOG("socket!\n");
	TRY((sock_s = socket(AF_INET, SOCK_STREAM, 0)), "socket");

	// parametros conexion
	memset(&ads, 0, sizeof(ads));
	ads.sin_family = AF_INET;
	ads.sin_port = htons(PORT);
	ads.sin_addr.s_addr = inet_addr(IP);

	// permitimos reutilizacion ip:port (time_wait)
	LOG("setsockopt!\n");
	TRY(setsockopt(sock_s, SOL_SOCKET, SO_REUSEADDR,
		(const char *) &on, sizeof(on)), "setsockopt");

	// bind a ip:puerto
	LOG("bind!\n");
	TRY((bind(sock_s, (struct sockaddr *)&ads, sizeof(ads))), "bind");

	// modo escucha
	LOG("listen!\n");
	TRY(listen(sock_s, 1), "listen");

	// aceptamos conexiones
	LOG("accept!\n");
	len = sizeof(adc);
	TRY((sock_c = accept(sock_s, (struct sockaddr *)&adc, &len)), "acept");

	// inicializamos rfd
	FD_ZERO(&rfd);

	// IAC WILL ECHO
	write(sock_c, "\xff\xfb\x01", 3);
	read(sock_c, sbuf, 3);

	// IAC WILL suppress go-ahead (33)
	write(sock_c, "\xff\xfb\x03", 3);
	read(sock_c, sbuf, 3);
/*
	// hasta signal
	while (1)
	{
		LOG("read socket <-> term!\n");

		FD_SET(term_m, &rfd);
		FD_SET(sock_c, &rfd);

		// i/o check
		r = select((term_m > sock_c ? term_m : sock_c) + 1, 
			&rfd, 0, 0, NULL);

		// preparado ?
		if (r > 0 || errno == EINTR) 
		{
			// lectura socket
			if (FD_ISSET(sock_c, &rfd))
			{
				// sock -> term
				if ((sz = read(sock_c, sbuf, SZ_SBUF)) > 0) 
					write(term_m, sbuf, sz);

				// salimos ?
				else break;
			}

			// lectura tty
			if (FD_ISSET(term_m, &rfd))
			{
				// term -> sock
				if ((sz = read(term_m, sbuf, SZ_SBUF)) > 0) {
					write(sock_c, sbuf, sz);
					dumphex(sbuf, sz);
				}
			}
		}
	}


	// nunca deberia llegar 
	return 0;
}
