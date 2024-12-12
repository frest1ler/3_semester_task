#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


ssize_t mygetline(char * buf, int size){
    int fp_in = 0;
    return read(fp_in, buf, size-1);
}


int parse(char* buf, char** scripts){
    char *str1, *token;
    char *saveptr1;
    int j = 0;
    for (str1 = buf; ;j++, str1 = NULL) {
        token = strtok_r(str1, "|", &saveptr1);
        if (token == NULL)
            break;
        scripts[j] = strdup(token);
    }
    return j;
}

int myexit(int n){
    exit(n);
}

int exec_command(int n, char** scripts){
    int pipefd[n-1][2];
    for (int i = 0; i < n - 1; i++) {
        if (pipe(pipefd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }


    for (int i = 0; i < n; i++){
        char *str2, *subtoken;
        char *saveptr2;
        char* myargv[128];
        int j = 0;

        for (str2 = scripts[i]; ;j++, str2 = NULL) {
            subtoken = strtok_r(str2, " \t", &saveptr2);
            if (subtoken == NULL)
                break;
            myargv[j] = strdup(subtoken);
        }
        myargv[j] = NULL; // Завершающий ноль
        pid_t p = fork();
        if (p == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (p == 0){ // текущий дочерний процесс
            // перенаправляем потоки
            if(i > 0){
                // Перенаправляем входной поток с предыдущего канала
                close(pipefd[i-1][1]);
                if(dup2(pipefd[i-1][0], 0) < 0){
                    perror("dup");
                    exit(EXIT_FAILURE);
                }
                close(pipefd[i-1][0]);
            }
            if(i < n-1){ // если не последний
                // Перенаправляем выходной поток на следующий канал
                close(pipefd[i][0]);
                if(dup2(pipefd[i][1], 1)<0){
                    perror("dup");
                    exit(EXIT_FAILURE);
                }
                close(pipefd[i][1]);
            }
            // исполняем
            // printf("%s,%s.\n", myargv[0], myargv[1]);
            if (execvp(myargv[0], myargv) == -1){
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
        else{ // Родительский процесс
            if (i > 0) {
                close(pipefd[i-1][0]);
                close(pipefd[i-1][1]);
            }
            for (int k = 0; k < j; k++) {
              free(myargv[k]);
            }
        }
    }

    for (int j = 0; j < n; j++) {
        wait(NULL);
    }

    return 0;
}

int main(){
    while(1){
        if (write(1, "$ ", 2) != 2){
            perror("write");
            exit(EXIT_FAILURE);
        }
        int size = 1024;
        char buf[size];
        if(mygetline(buf, size) <= 0){ // EOF
            break;
        }
        buf[strcspn(buf, "\n")] = 0;  // Remove newline
        char* scripts[size/16];
        int n = parse(buf, scripts);
        exec_command(n, scripts);
    }
    return 0;
}
