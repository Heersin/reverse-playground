#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define PE_POINTER 0x3C
#define MAX_CAP 4096

#define SEC_TYPE_STRING 0x01u
#define SEC_TYPE_CODE 0x02u

typedef unsigned int addr_t;
unsigned char bytes_buf[MAX_CAP];

struct bin_hdr {
	unsigned char name[16];
	unsigned char info[16];
	unsigned char build_time[16];
	unsigned int entry_cnt;
};

struct entry {
	unsigned char entry_type;
	unsigned char is_enable;
	unsigned char reserve;
	unsigned char reserve2;
	unsigned int entry_offset;
	unsigned int entry_size;
};

typedef struct bin_hdr BinHdr;
typedef struct entry Entry;

#define NEW(x) new_hdr(sizeof(x))
#define FREE(x) free_hdr(x);x = NULL

void put_dos_hdr(const char *resrc_name, FILE *fp)
{
	FILE *srcp = fopen(resrc_name, "rb");
	if (srcp == NULL)
	{
		perror("No such file");
		return;
	}
	
	memset(bytes_buf, 0x0, MAX_CAP);
	fread(bytes_buf, 1, MAX_CAP, srcp);
	fwrite(bytes_buf, 1, 0x100, fp);

	fclose(srcp);
}

void append_resrc(const char *resrc_name, FILE *fp)
{
	FILE *srcp = fopen(resrc_name, "rb");
	if (srcp == NULL)
	{
		perror("No such file");
		return;
	}
	
	memset(bytes_buf, 0x0, MAX_CAP);
	fread(bytes_buf, 1, MAX_CAP, srcp);
	fwrite(bytes_buf, 1, MAX_CAP, fp);

	fclose(srcp);
}

int main(int argc, char **argv)
{
	const char *filename = "test.dump";
	FILE *fp = fopen(filename, "wb");

	// 1. Add DOS Header
	put_dos_hdr("./resrc/pure_dos_hdr", fp);

	// 2. Baisc info
	BinHdr basic;
	basic.name[0] = 'h';
	basic.name[1] = 'e';
	basic.name[2] = 'l';
	basic.name[3] = 'l';
	basic.name[4] = 'o';
	basic.name[5] = '.';
	basic.name[6] = 'e';
	basic.name[7] = 'x';
	basic.name[8] = 'e';
	basic.name[9] = 0x00;

	basic.info[0] = 'c';
	basic.info[1] = 'o';
	basic.info[2] = 'm';
	basic.info[3] = 'p';
	basic.info[4] = 'i';
	basic.info[5] = 'l';
	basic.info[6] = 'e';
	basic.info[7] = 'd';
	basic.info[8] = ' ';
	basic.info[9] = 'b';
	basic.info[10] = 'y';
	basic.info[11] = ' ';
	basic.info[12] = 'A';
	basic.info[13] = 'U';
	basic.info[14] = 'x';
	basic.info[15] = 0x00;

	basic.build_time[0] = 'b';
	basic.build_time[1] = 'i';
	basic.build_time[2] = 'l';
	basic.build_time[3] = 'i';
	basic.build_time[4] = ':';
	basic.build_time[5] = '6';
	basic.build_time[6] = '7';
	basic.build_time[7] = '2';
	basic.build_time[8] = '3';
	basic.build_time[9] = '2';
	basic.build_time[10] = '8';
	basic.build_time[11] = '0';
	basic.build_time[12] = '9';
	basic.build_time[13] = '4';
	basic.build_time[14] = 0x00;
	
	basic.entry_cnt = 4;

	fwrite(&basic, 1, sizeof(BinHdr), fp);

	Entry example_code, flag, jiaran, example_chart;
	example_code.entry_type = SEC_TYPE_CODE;
	example_code.is_enable = 1;
	example_code.entry_offset = 0x1000;
	example_code.entry_size = 0x1000;
	fwrite(&example_code, 1, sizeof(Entry), fp);

	flag.entry_type = SEC_TYPE_CODE;
	flag.is_enable = 0;
	flag.entry_offset = 0x2000;
	flag.entry_size = 0x1000;
	fwrite(&flag, 1, sizeof(Entry), fp);

	jiaran.entry_type = SEC_TYPE_STRING;
	jiaran.is_enable = 0;
	jiaran.entry_offset = 0x3000;
	jiaran.entry_size = 0x1000;
	fwrite(&jiaran, 1, sizeof(Entry), fp);

	example_chart.entry_type = SEC_TYPE_STRING;
	example_chart.is_enable = 1;
	example_chart.entry_offset = 0x4000;
	example_chart.entry_size = 0x1000;
	fwrite(&example_chart, 1, sizeof(Entry), fp);

	fseek(fp, 0x1000, SEEK_SET);

	append_resrc("./resrc/example.code", fp);
	append_resrc("./resrc/flag.code", fp);
	append_resrc("./resrc/jiaran.txt", fp);
	append_resrc("./resrc/windows.txt", fp);

	fclose(fp);
	return 0;
}


























