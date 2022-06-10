#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/wait.h>

#define BUFSZ 512

// 20220131
// new_ye@r -> ?6H0J6oC
const char *raw_table = "a^?(6)BN*&H;=+0yJ{:I@619->ronCpP";
const char *alphabet = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

const char final_buf[128] = {};

void print_x(char *buf, int len)
{
	for (int i = 0; i < len; ++i)
	{
		printf("0x%X, ", buf[i]);
	}
	puts("");
}

void print(char *buf, int len)
{
	for (int i = 0; i < len; ++i)
	{
		printf("%c, ", buf[i]);
	}
	puts("");
}

// decode function for rot47
void re(char *secret)
{
	int rot = 47;
	int tb_len = strlen(alphabet);

	for (int i = 0; i < 8; ++i)
	{
		char ch = secret[i];
		int index = strchr(alphabet, ch) - alphabet;
		int ori_index = (index + rot) % tb_len;
		secret[i] = alphabet[ori_index];
	}

	secret[8] = 0x00;
}

void pick(char *buf, char *hint, int len)
{
	for (int i = 0; i < len; ++i)
	{
		int index = i * 4 + hint[i];
		buf[i] = raw_table[index];
	}

	buf[len] = 0x00;
}

void lock()
{
	FILE *fp;
	fp = fopen("../.info/lock", "w");
	if (!fp) {
		perror("Sys Failed");
		exit(2);
	}
	fclose(fp);
}

int fake(char *data, int len)
{
    int shmid;
    key_t key;
    char *shmadd;
 
    //创建key值
    key = ftok("../.info/key", 2022);
    if(key == -1)
    {
        perror("ftok");
    }

    //创建共享内存
    shmid = shmget(key, BUFSZ, IPC_CREAT|0666);    
    if(shmid < 0)
    {
        perror("shmget");
        exit(-1);
    }
 
    //映射
    shmadd = shmat(shmid, NULL, 0);
    if(shmadd < 0)
    {
        perror("shmat");
        exit(-1);
    }
 
    bzero(shmadd, BUFSZ); // 共享内存清空
    for (int i = 0; i < len; ++i)
    {
	    shmadd[i] = data[i];
    }
 
    return 0;
}

void report()
{
	chdir("..");
	execl("./game", NULL);
}

int main(int argc, char **argv)
{
	if (!strstr(argv[0], "game_helper"))
	{
		puts("An unkown box, wizard may know the usage of it");
		puts("Where is the wizard?");
		exit(0);
	}

	char input[8] = {};
	puts("Enter password to unlock game helper :");
	scanf("%8s", &input);
	for (int i = 0; i < 8; ++i)
	{
		input[i] = input[i] - '0';
	}

	int pid = -1;
	pid = fork();
	char temp_buf[128] = {};

	if (pid < 0)
	{
		// failed to exec fork
		exit(1);
	}

	// child
	if (pid == 0)
	{
		/// do something
		signal(SIGUSR1, report);
		sleep(10);
		exit(0);
	}
	else
	{
		/// play game
		/// preparing the string
		lock();
		pick(temp_buf, input, 8);
		re(temp_buf);
		fake(temp_buf, 9);
	}

	sleep(1);
	kill((pid), SIGUSR1);
	wait(NULL);

	return 0;
}




