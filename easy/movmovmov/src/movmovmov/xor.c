#include <stdio.h>
#include <stdlib.h>

int main() {
	const char *flag = "flag{movmovmeow}";
	unsigned char data[22] = {};
	int n = 16;
	for (int i = 0; i < n; ++i) {
		unsigned char xor_k = 42;
		unsigned char xor_byte = flag[i] ^ xor_k;
		printf("%c ^ 0x%x = 0x%x\n", flag[i], xor_k, xor_byte);
		data[i] = xor_byte;
	}

	for (int i = 0; i < n; ++i) {
		printf("0x%0x, ", data[i]);
	}
	return 0;
}
