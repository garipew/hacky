#include <unistd.h>

int main(){
	char *argv[] = {"/bin/bash", "-p", NULL};
	execve("/bin/bash", argv, NULL);
	return 1;
}
