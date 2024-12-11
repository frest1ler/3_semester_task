#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char* argv[]){
    for (int i = 1; i < argc; i++){
        int p = fork();
        if (p == 0){
            usleep(2000*atoi(argv[i]));
            printf("%d\n", atoi(argv[i]));
            exit(0);
        }
    }
    for (int i = 0; i < argc; i++){
        int s = 0;
        wait(&s);
    }
    return 0;
}
