#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void dump(const unsigned char*, unsigned int);

int main(){
	int sockfd;
	int recv_len;
	unsigned char buffer[9000];
	if((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0){
		perror("at socket");
		exit(1);
	}
	for(int i = 0; i < 3; i++){
		recv_len = recv(sockfd, buffer, 9000, 0);
		if(recv_len == 0){
			printf("Connection closed by host\n");
			return 0;
		}else if(recv_len < 0){
			perror("at recv");
			exit(2);
		}
		printf("recvd a %d bytes packet\n", recv_len);
		dump(buffer, recv_len);
	}
	return 0;
}

void dump(const unsigned char* buf, unsigned int len){
	unsigned char byte;
	int i, j;
	for(i = 0; i < len; i++){
		byte = buf[i];
		printf("%02x ", buf[i]);

		if((i%16==15) || (i==len-1)){
			for(j = 0; j < 15-(i%16); j++){
				printf("   ");
			}
			printf("| ");
			for(j=i-(i%16); j <= i; j++){
				byte = buf[j];
				if(byte > 31 && byte < 127){
					printf("%c", byte);
				}else{
					printf(".");
				}
			}
			printf("\n");
		}
	}
}
