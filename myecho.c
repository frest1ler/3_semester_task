#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[]){
	if (argc < 2){
		printf("\n");
		return 0;
	}
	int flag = 0;
	int i = 1;
	if (strcmp(argv[1], "-n") == 0){ 
		i++; 
		flag = 1;
	}

	for (; i < argc; i++){
		printf("%s", argv[i]);
		if (i < argc - 1) printf(" ");
	}
	if (!flag) 
		printf("\n");
	return 0;
}
