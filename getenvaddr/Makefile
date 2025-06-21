ARCH?=32

all: getenvaddr printenvaddr

getenvaddr:
	gcc -m$(ARCH) -o getenvaddr getenvaddr.c
	
printenvaddr:
	gcc -o printenvaddr printenvaddr.c

clean:
	rm -rf getenvaddr printenvaddr
