#include <stdlib.h>
#include <stdio.h>

const char *flag_nearby = "xored flag nearby";
unsigned char data[24] = {
	0x4c, 0x46, 0x4b, 0x4d, 0x51, 0x47, 0x45, 0x5c, 0x47, 0x45, 0x5c, 0x47, 0x4f, 0x45, 0x5d, 0x57
};
const char *gift = "gift :";

int main(){
	unsigned char input[24];
	scanf("%16s", input);
	int i;
	for (i = 0; i < 16; ++i)
	{
		if (data[i] != (input[i] ^ 42)) {
			putchar('\n');
			return 1;
		}
		else
			putchar(input[i]);
	}
	putchar('\n');
	printf("Congrats FLAG : %s\n", input);
}
