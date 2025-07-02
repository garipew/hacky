#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h> 
#include <stdlib.h>

#define CHR "%_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-"

int main(int argc, char** argv){
	unsigned targ, last, t[4], l[4];
	unsigned try, single, carry;
	int len, i, j, k, m, flag;
	char word[3][4];
	unsigned char mem[70];
	if(argc < 2){
		printf("Usage: %s <start> <end>\n", argv[0]);
		return 1;
	}
	srand(time(NULL));
	memset(mem, 0, 70);
	strcpy(mem, CHR);
	len = strlen(mem);
	strfry(mem);
	last = strtoul(argv[1], NULL, 0);
	targ = strtoul(argv[2], NULL, 0);

	t[0] = targ & 0xff;
	t[1] = (targ & 0xff00) >> 8;
	t[2] = (targ & 0xff0000) >> 16;
	t[3] = (targ & 0xff000000) >> 24;

	l[0] = last & 0xff;
	l[1] = (last & 0xff00) >> 8;
	l[2] = (last & 0xff0000) >> 16;
	l[3] = (last & 0xff000000) >> 24;

	for(int value = 1; value < 5; value++){
		carry = flag = 0;
		for(int byte = 0; byte < 4; byte++){
			for(i = 0; i < len; i++){
				for(j = 0; j < len; j++){
					for(k = 0; k < len; k++){
						for(m = 0; m < len; m++){
							if(value < 2 ) j = len+1;
							if(value < 3) k = len +1;
							if(value < 4) m = len+1;
							try = t[byte] + carry+mem[i]+mem[j]+mem[k]+mem[m];
							single = (try & 0xff);
							if(single == l[byte]){
								carry = (try & 0xff00) >> 8;
								if(i < len) word[0][byte] = mem[i];
								if(j < len) word[1][byte] = mem[j];
								if(k < len) word[2][byte] = mem[k];
								if(m < len) word[3][byte] = mem[m];
								i = j = k = m = len+2;
								flag++;
							}
						}
					}
				}
			}
		}
		if(flag == 4){
			for(i = 0; i < value; i++){
				printf("0x%08x\n", *((unsigned int*)word[i]));
			}
			return 0;
		}
	}
	return 0;
}
