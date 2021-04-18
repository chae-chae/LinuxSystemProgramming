#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

char type(mode_t);

char* perm(mode_t);

void printStat(char*, char*, struct stat*);

void printInode(char *, char *, struct stat*);
void printLs(char *pathname, char *file, struct stat *st);
void sortTime(char *pathname, char *file, struct stat *st);
void saveList(char *pathname, char *file, struct stat *st);

int count = 0;
struct stat arr[100]; // count로 malloc
int idx = 0;

struct Data
{
    long second;
    char *filename;
};

struct Data data[100];

int main(int argc, char **argv){
    DIR *dp;
    char *dir;
    struct stat st;
    struct dirent *d;
    char path[1024];

    if (argc == 1) // ls
    {
        dir = ".";
        if ((dp = opendir(dir)) == NULL) perror(dir);

        while ((d = readdir(dp)) != NULL){
            sprintf(path, "%s/%s", dir, d->d_name);
            if (lstat(path, &st) < 0){
                perror(path);
            }
            printLs(path, d->d_name, &st);
        }
    } else if (!strcmp("-i", argv[1])){ // -i option
        if (argc == 3){ // dirname or filename given
            dir = argv[2];
            sprintf(path, "./%s", dir);
            if (lstat(path, &st) < 0) perror(path);
            printInode(path, path, &st);
            if (type(st.st_mode) != 'd') // filename
            {
                exit(0);
            }
        } else { // just i option
            dir = ".";
        }
        if ((dp = opendir(dir)) == NULL) perror(dir);
        while ((d = readdir(dp)) != NULL){
            sprintf(path, "%s/%s", dir, d->d_name);
            if (lstat(path, &st) < 0){
                perror(path);
            }
            printInode(path, d->d_name, &st);
        }
        closedir(dp);
        return 0;

    } else if (!strcmp("-l", argv[1])){ // -l option
        if (argc == 3){ // dirname or filename given
            dir = argv[2];
            sprintf(path, "./%s", dir);
            if (lstat(path, &st) < 0) perror(path);
            printStat(path, path, &st);
            if (type(st.st_mode) != 'd') // filename
            {
                exit(0);
            }
        } else {
            dir = ".";
        }
        if ((dp = opendir(dir)) == NULL) perror(dir);
        while ((d = readdir(dp)) != NULL){
            sprintf(path, "%s/%s", dir, d->d_name);
            if (lstat(path, &st) < 0){
                perror(path);
            }
            printStat(path, d->d_name, &st);
        }
        closedir(dp);
        return 0;
    
    } else if (!strcmp("-t", argv[1])){ // -t option
        if (argc == 3){ // dirname or filename given
            dir = argv[2];
            sprintf(path, "./%s", dir);
            if (lstat(path, &st) < 0) perror(path);
            sortTime(path, path, &st);
            count++;
            if (type(st.st_mode) != 'd') // filename
            {
                exit(0);
            }
        } else {
            dir = ".";
        }
        if ((dp = opendir(dir)) == NULL) perror(dir);
        while ((d = readdir(dp)) != NULL){
            sprintf(path, "%s/%s", dir, d->d_name);
            if (lstat(path, &st) < 0){
                perror(path);
            }
            if (!strcmp(".", d->d_name) || !strcmp("..", d->d_name)){
                continue;
            }
            saveList(path, d->d_name, &st);
            count++;
        }
        sortTime(path, d->d_name, &st);
        closedir(dp);
        return 0;
    } else if (argc == 2){
        dir = argv[1];
        sprintf(path, "./%s", dir);
        if (lstat(path, &st) < 0) perror(path);
        printLs(path, path, &st);
        if (type(st.st_mode) != 'd') // filename
        {
            exit(0);
        }
        if ((dp = opendir(dir)) == NULL) perror(dir);
        while ((d = readdir(dp)) != NULL){
            if (lstat(path, &st) < 0){
                perror(path);
            }
            printLs(path, d->d_name, &st);
        }
    } else {
    closedir(dp);
    return 0;
    }
}

char type(mode_t mode){
    if (S_ISREG(mode)) return('-');
    if (S_ISDIR(mode)) return('d');
    if (S_ISCHR(mode)) return('c');
    if (S_ISBLK(mode)) return('b');
    if (S_ISLNK(mode)) return('l');
    if (S_ISFIFO(mode)) return('p');
    if (S_ISSOCK(mode)) return('s');
}

char* perm(mode_t mode){
    int i;
    static char perms[10] = "---------";
    for (int j = 0; j < 9; j++){
        perms[j] = '-';
    }
    perms[9] = '\0';
    
    for (i = 0; i < 3; i++){
        if (mode & (S_IRUSR >> i*3)) {
            perms[i*3] = 'r';
        }
        if (mode & (S_IWUSR >> i*3)) {
            perms[i*3+1] = 'w';
        }
        if (mode & (S_IXUSR >> i*3)) {
            perms[i*3+2] = 'x';
        }
    }
    return(perms);
}

void printStat(char *pathname, char *file, struct stat *st){
    if (!strcmp(".", file)){
        return;
    }
    if (!strcmp("..", file)){
        return;
    }
    printf("%c%s ", type(st->st_mode), perm(st->st_mode));
    printf("%3ld ", st->st_nlink);
    printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);
    printf("%9ld ", st->st_size);
    printf("%.12s ", ctime(&st->st_mtime) + 4);
    printf("%s\n", file);
}

void printInode(char *pathname, char *file, struct stat *st){
    if (!strcmp(".", file)){
        return;
    }
    if (!strcmp("..", file)){
        return;
    }
    printf("%ld %s\n", st->st_ino, file);
}

void printLs(char *pathname, char *file, struct stat *st){
    if (!strcmp(".", file)){
        return;
    }
    if (!strcmp("..", file)){
        return;
    }
    printf("%s\n", file);
}

void saveList(char *pathname, char *file, struct stat *st){
    data[idx].second = st->st_mtime;
    data[idx].filename = file;
    idx++;
}

void sortTime(char *pathname, char *file, struct stat *st){
    
    long temp = data[0].second;
    char * tmp = NULL;
    int max;
    
    for (int i = 0; i < count - 1; i++)
    {
        max = i;
        for (int j = i + 1; j < count; j++)
        {
            if (data[j].second > data[max].second){
                max = j;
            }
        }
        temp = data[max].second; // 큰 거 복사
        tmp = data[max].filename;
        data[max].second = data[i].second;
        data[max].filename = data[i].filename;            
        data[i].second = temp;
        data[i].filename = tmp;
    }
    
    for (int i = 0; i < count; i++)
    {
        printf("%s\n", data[i].filename);
    }
    
}