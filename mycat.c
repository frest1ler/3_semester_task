#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int mycat(int fp_in, int fp_out){
    size_t size = 1024;
    char buf[size];
    while(1){
        ssize_t n = read(fp_in, buf, size);
        if(n > 0){
            ssize_t k = 0;
            while(n > 0){
                k = write(fp_out, buf + k, n);
                if (k < 0){
                    perror("writing failed");
                    return 0;
                }
                n -= k;
            }
        }
        else if(n == 0){
            if (fp_in){
                close(fp_in);
            }
            return 0;
        }
        else{
            perror("error reading from stdin");
            exit(1);
        }
    }
    return 0;
}

int main(int argc, char * argv[]){
    if (argc == 1){
        mycat(0, 1);
    }
    else{
        for (int i=1;i< argc;i++){
            int  fp;
            fp = open(argv[i], O_RDONLY);
            if (fp < 0){
                perror("opening failed");
                return 0;
            }
            else{
                mycat(fp, 1);
            }
        }
    }
    return 0;
}
