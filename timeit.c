#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

struct timespec time0;
struct timespec time1;

int main(int argc, char* argv[]){
    clock_gettime(CLOCK_REALTIME, &time0);
    int p = fork();
    if (p == 0){
        if (execvp(argv[1], &argv[2]) == -1)
            printf("error\n");
    }

    else{
        int s = 0;
        wait(&s);
        clock_gettime(CLOCK_REALTIME, &time1);
        printf("exec_time = %f мс\n", 1000*time1.tv_sec + 0.000001*time1.tv_nsec - 1000*time0.tv_sec - 0.000001*time0.tv_nsec);
    }

} 
