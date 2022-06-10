#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define PE_POINTER 0x3C
#define MAX_CAP 4096

#define OP_NOP 0x00
#define OP_SET 0x01
#define OP_ADD 0x02
#define OP_NEXT 0x03
#define OP_PRINT 0x04

#define SEC_TYPE_STRING 0x01u
#define SEC_TYPE_CODE 0x02u

typedef unsigned int addr_t;
unsigned char bytes_buf[MAX_CAP];

// sizeof bin_hdr : 52 B
struct bin_hdr {
	unsigned char name[16];
	unsigned char info[16];
	unsigned char build_time[16];
	unsigned int entry_cnt;
};

// sizeof entry_hdr : 12 B
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

void *new_hdr(size_t size)
{
	void *p = malloc(size);
	memset(p, 0x00, size);
	return p;
}

void free_hdr(void *p)
{
	if (!p)
		return;
	free(p);
}

#define NEW(x) new_hdr(sizeof(x))
#define FREE(x) free_hdr(x);x = NULL

addr_t locate_start(FILE *fp)
{
	addr_t real_start;
	fseek(fp, PE_POINTER, SEEK_SET);
	fread(&real_start, 1, sizeof(addr_t), fp);
	return real_start;
}

BinHdr *read_basic_info(FILE *fp)
{
	BinHdr *binary = NEW(BinHdr);
	fread(binary, 1, sizeof(BinHdr), fp);
	return binary;
}

Entry *read_entry(FILE *fp)
{
	Entry *entry = NEW(Entry);
	fread(entry, 1, sizeof(Entry), fp);
	return entry;
}


void read_section(FILE *fp, Entry *entry)
{
	if (entry->entry_size > MAX_CAP)
	{
		printf("entry size : 0x%X\n", entry->entry_size);
		perror("Failed to read entry");
		return;
	}

	addr_t sec_offset = entry->entry_offset;
	fseek(fp, sec_offset, SEEK_SET);
	fread(bytes_buf, 1, MAX_CAP, fp);
}

void output_screen(unsigned char *mem, int len)
{
	for (int i = 0; i < len; ++i)
	{
		char ch = mem[i];
		putchar(ch);
	}

	putchar('\n');
}

void run_as_code_section()
{
	unsigned char ch;
	unsigned char is_data_mode = 0x1;
	unsigned char reg;
	unsigned char mem[64] = {0};
	unsigned char p = 0;

	for (int i = 0; i < MAX_CAP; i += 2) {
		// decode
		unsigned char xor_k = bytes_buf[i];
		unsigned char raw_b = bytes_buf[i + 1];
		ch = xor_k ^ raw_b;

		// exec
		if (is_data_mode) {
			is_data_mode = !is_data_mode;
			reg = ch;
			continue;
		}

		switch (ch)
		{
			case OP_SET:
				mem[p] = reg;
				break;
			case OP_ADD:
				mem[p] += reg;
				break;
			case OP_NEXT:
				p += reg;
				break;
			case OP_PRINT:
				output_screen(mem, p);
				break;
			case OP_NOP:
			default:
				break;
		}

		is_data_mode = !is_data_mode;
	}
}


void run_as_ascii_art()
{
	int width = 61;
	for (int i = 0; i < MAX_CAP; ++i)
	{
		char ch = bytes_buf[i];
		if (ch == 0)
			return;
		putchar(ch);
		if (i % width == 0)
			putchar('\n');
	}
}

void run_section(FILE *fp, Entry *entry)
{
	if (!entry->is_enable)
		return;

	read_section(fp, entry);
	if (entry->entry_type == SEC_TYPE_STRING)
	{
		run_as_ascii_art();
		return;
	}

	if (entry->entry_type == SEC_TYPE_CODE)
	{
		run_as_code_section();
		return;
	}

	perror("Unknown type");
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage : loader example.exe\n");
		return 0;
	}

	const char *filename = argv[1];
	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		printf("No such file %s\n", filename);
		return 1;
	}

	addr_t real_start = locate_start(fp);
	fseek(fp, real_start, SEEK_SET);

	// parse basic info
	BinHdr *file_header = read_basic_info(fp);
	printf("[Name]: %s\n[Info]: %s\n[Build]: %s\n",
                        file_header->name,
                        file_header->info,
                        file_header->build_time);


	unsigned int entry_cnt = file_header->entry_cnt;
	if (entry_cnt > 16) {
		printf("Error, too many entries : %d\n", entry_cnt);
		FREE(file_header);
		fclose(fp);
		return 1;
	}

	addr_t cur;
	for (int i = 0; i < entry_cnt; ++i)
	{
		Entry *temp_entry = read_entry(fp);
		cur = ftell(fp);
		run_section(fp, temp_entry);
		fseek(fp, cur, SEEK_SET);
		FREE(temp_entry);
	}

	FREE(file_header);
	fclose(fp);
	return 0;
}


























