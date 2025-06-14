#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv){
	if(argc < 3){
		printf("Usage: %s <program_name> <env>\n", argv[0]);
		return 1;
	}
	char* start;	
	int name_len;
	if((start = strrchr(argv[0], '/')) != NULL){
		name_len = strlen(start) - 1;
	} else{
		name_len = strlen(argv[0]);
	}
	int distance = name_len - strlen(argv[1]);
	printf("on %s, %s is at %p\n", argv[1], argv[2],
		 	getenv(argv[2]) + (2*distance));
	return 0;
}
