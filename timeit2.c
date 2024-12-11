#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct timespec time0;
struct timespec time1;

int find_params(char * buf){
    int count1 = 1;
    int count2 = 0;
    for(int i=0; i < strlen(buf); i++){ 
        if(buf[i] == ' ' && buf[i+1] && buf[i+1] != ' ' && buf[i+1] != '\t' && buf[i+1] != '\n'){
            count1++;
        }
        else if(buf[i] == '\n'){
            if(buf[i+1] && buf[i+1] != ' ' && buf[i+1] != '\t' && buf[i+1] != '\n'){
                count1++;
            }
            count2++;
        }
    }
    printf("\t%d\t%d\t%ld\n", count2, count1, strlen(buf));
    return 0;
}

int main(int argc, char* argv[]){
    clock_gettime(CLOCK_REALTIME, &time0);
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    int p = fork();
    if (p == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (p == 0){
        close(1);
        if(dup(pipefd[1]) < 0){
            perror("dup");
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
        if (execvp(argv[1], &argv[1]) == -1){
            printf("error\n");
            exit(EXIT_FAILURE);
        }
    }
    else{
        close(pipefd[1]);
        int s = 0;
        wait(&s);
        clock_gettime(CLOCK_REALTIME, &time1);
        printf("exec_time = %f мс\n", 1000*time1.tv_sec + 0.000001*time1.tv_nsec - 1000*time0.tv_sec - 0.000001*time0.tv_nsec);
        size_t size = 10*1024;
        char buf1[size];
        read(pipefd[0], buf1, size);
        close(pipefd[0]);
        buf1[size - 1] = 0;
        find_params(buf1);
        close(pipefd[1]);
    }
    return 0;
} 
