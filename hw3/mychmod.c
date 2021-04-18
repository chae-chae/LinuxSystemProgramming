#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
    unsigned int input;
    sscanf(argv[1], "%o", &input);
    chmod(argv[2], input);
    return 0;
}