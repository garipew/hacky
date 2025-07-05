#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFFSIZE 500

void my_func(char* str, char* addr){
	char addr_buff[4];
	char buff[BUFFSIZE];
	printf("buff at %p\n", buff);
	strcpy(buff, str);
	printf("%s", buff);
	if(addr){
		strcpy(addr_buff, addr);
	}
}

int main(int argc, char** argv){
	#ifdef PRINT
	if(argc < 3){
		fprintf(stderr, "Usage: %s <string> <address string>\n", argv[0]);
		return 1;
	}
	for(int i = 0; i < strlen(argv[1]); i++){
		if(!isprint(argv[1][i])){
			fprintf(stderr, "Error! Only printable characters are allowed.\n"); 
			fprintf(stderr, "argv[1][%d] = 0x%2x is not printable.\n", i, argv[1][i] & 0xff);
			fprintf(stderr, "Exiting...\n"); 
			exit(2);
		}
	}
	#else
	if(argc < 2){
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		return 1;
	}
	#endif
	#ifdef FROMMAIN
	char buff[BUFFSIZE];
	printf("buff at %p, from main\n", buff);
	strcpy(buff, argv[1]);
	printf(buff);
	#else
	my_func(argv[1], argv[2]);
	#endif
	return 0;
}
