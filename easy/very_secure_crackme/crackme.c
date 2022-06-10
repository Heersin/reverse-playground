#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

const char *filename = "temp.py";

int check_py()
{
	FILE *fp = NULL;
	char buffer[128];
	fp = popen("python3 --version", "r");
	fgets(buffer, sizeof(buffer), fp);
	pclose(fp);

	if (strstr(buffer, "Python") == NULL)
		return FALSE;

	return TRUE;
}

int gen_py()
{
	int offset = 0x4000;
	FILE *self = fopen("crackme", "rb");
	if (!self)
	{
		printf("Broken Broken Broken\n");
		return FALSE;
	}

	FILE *py = fopen(filename, "wb");
	if (!py)
	{
		printf("Failed Failed Failed\n");
		fclose(self);
		return FALSE;
	}

	fseek(self, offset, SEEK_SET);
	fseek(py, 0, SEEK_SET);

	while(TRUE)
	{
		int ch = fgetc(self);
		if (feof(self))
			break;
		fputc(ch, py);
	}

	fclose(py);
	fclose(self);

	return 0;
}

int rm_py()
{
	if (remove(filename) != 0)
	{
		printf("Failed to remove\n");
		return FALSE;
	}

	return TRUE;
}

int run_py()
{
	system("python3 temp.py");
	return 0;
}

int main()
{
	if (!check_py())
	{
		printf("Python Environment Not Found\n");
		return 1;
	}

	gen_py();
	run_py();
	rm_py();

	return 0;
}
