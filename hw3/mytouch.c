#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utime.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
    struct stat statbuf;
    FILE *fp;
    if (stat(argv[1], &statbuf) < 0){
        fp = fopen(argv[1], "w");
        fclose(fp);
        return 0;
    };
    utime(argv[1], NULL);
    return 0;
}