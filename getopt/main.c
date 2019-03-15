#include <stdio.h>
#include <getopt.h>

int opt_parse(int argc, char **argv)
{
    while (1) {
            static struct option opts[] = {
                    {"init", 0, NULL, 'i'},
                    {"stop", 0, NULL, 's'},
                    {"pull", 1, NULL, 'p'},
                    {"push", 1, NULL, 'P'},

                    {"help", 0, NULL, 'h'},
                    {NULL, 0, NULL, 0}
            };

            int index = 0, c;

            c = getopt_long(argc, argv, "isp:P:h", opts, &index);
            if (c == -1)
                    break;
            switch (c) {
            case 'i':
                    printf("init\n");
                    break;
            case 's':
					printf("stop\n");
                    break;
            case 'p':
					printf("pull %d\n", atoi(optarg));
                    break;
            case 'P':
					printf("push %d\n", atoi(optarg));
                    break;
            case 'h':
                    fprintf(stderr, "Usage: %s [OPTIONS]\n", argv[0]);
                    fprintf(stderr, " --init,    -i            init\n");
                    fprintf(stderr, " --stop,    -s            stop\n");
                    fprintf(stderr, " --pull,    -p N          pull\n");
                    fprintf(stderr, " --push,    -P N          push\n");

                    fprintf(stderr, " --help,    -h            help\n");
                    exit (0);
                    break;
            default:
                    break;
            }
    }
}


int main(int argc, char **argv)
{
    opt_parse(argc, argv);
}
