
/******************************************************************
* pseudo-telnetd util ;)
* compile: 
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
#include <pty.h>




/*********************************
* macros aux, config
**********************************/

#define SHELL "/bin/sh"
#define SZ_SBUF 512





#if 0
#define dbg_printf printf
#else 
#define dbg_printf {;}
#endif

int my_login_tty(int fd)
{
	(void) setsid();
	if (ioctl(fd, TIOCSCTTY, (char *)NULL) == -1)
		return (-1);


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



/*********************************
* funciones 
**********************************/






// funcion principal
int main()
{
	int pid, term_m, term_s;

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

	my_openpty(&term_m, &term_s, NULL, &tt, &win);
	//signal(SIGTERM, sig);
	//signal(SIGINT, sig);
	//signal(SIGQUIT, sig);
	//signal(SIGCHLD, sig);

	// tercer fork 
	pid = fork();

	// hijo?
	if (pid == 0) //Child
	{
		// cerramos term_m
		close(term_m);

		// preparamos tty
		my_login_tty(term_s);
		// desactivamos echo
		tt.c_lflag &= ~ECHO;
		tcsetattr(0, TCSANOW, &tt);

		// activamos echo 
		//tt.c_lflag |= ECHO;
		//tcsetattr(0, TCSANOW, &tt);

		// lanzamos shell
		execv(SHELL, args);

		// llegados aqui, algo mal?
		kill(0, SIGTERM);
		exit(0);
	}

	write(term_m, "ls\n", 4);
	sleep(1);
	sz = read(term_m, sbuf, SZ_SBUF);
	printf("%s", sbuf);

	write(term_m, "mount\n", 6);
	sleep(1);
	sz = read(term_m, sbuf, SZ_SBUF);
	printf("%s", sbuf);

	// nunca deberia llegar 
	return 0;
}
