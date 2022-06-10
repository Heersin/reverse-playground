#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/ptrace.h>

#define BUFSZ 512

#define FLAG_LEN 8

char flag[9] = {'#', '#', '#', '#', '#', '#', '#', '#', 0x00};

int reveal_flag(char *shared)
{
	for (int i = 0; i < FLAG_LEN; ++i)
		flag[i] = shared[i];
}

int check_file_exs(const char *path)
{
	if (access(path, F_OK) != -1)
		return 1;
	return 0;
}

int check_debug()
{
	int debugger;
	if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == 0) {
		debugger = 0;
	} else {
        	debugger = 1;
    }
    return debugger;
}

void wizard_room()
{
	puts("You enter a room and meet a wizard");
	sleep(1);
	puts("\"Hi! I'm a wizard\"");
	sleep(1);
	puts("The 'unk_box' in your bag can reveal the answer to the ultimate question of life, the universe, and ...");
	sleep(1);
	puts("Your favorite thing -- flag");
	if (check_file_exs("./bag/game_helper"))
	{
		puts("Looks like you already have the game helper");
		puts("Usage is very simple : ./game_helper in ./bag directory");
		sleep(1);
		puts("See ya");
		exit(0);
	}

	puts("The true name of this 'unk_box' is game helper !");
	rename("./bag/unk_box", "./bag/game_helper");
	puts("Now check your '/bag', and find the flag with its help ~");
	sleep(1);
	puts("See ya");
	exit(0);
}

void victory_room()
{
	puts("You defeat the dragon");
	puts("And find a note in dragon's lair");
	sleep(1);
	puts("note : 20220131");
	puts("Is it a flag ?");
	exit(0);
}

void dragon_room()
{
	int your_hp = 10;
	int dragon_damage = 9999;
	puts("You move to the next room, a dragon is staring at you");
	puts("Dragon spews fire at you");
	sleep(1);
	if (your_hp < dragon_damage) 
	{
		printf("You are dead\n");
		exit(0);
	}
	victory_room();
}

void init_room()
{
	if (check_debug())
	{
		puts("You are cheating!!");
		sleep(2);
		puts("But I love cheating");
		sleep(1);
		wizard_room();
	}

	puts("You are looking for flag");
	sleep(1);
	puts("But unfortunately, you are lost");
	sleep(1);
	puts("Move forward ? [Y/N]");
	sleep(3);
	puts("(in fact you have no choice in this game)");
	puts("You move forward");
	sleep(1);
	dragon_room();
}

int my_ipc_child()
{
    int shmid;
    int ret;
    key_t key;
    char *shmadd;

    if (check_file_exs("./.info/lock") == 0)
    {
	    // not exist, not called by write_cli
	    return 1;
    }

    //创建key值
    key = ftok("./.info/key", 2022);
    if(key == -1)
    {
        perror("ftok");
    }
 
    // system("ipcs -m"); //查看共享内存
 
    //打开共享内存
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
 
    // 读共享内存区数据
    // printf("data = [%s]\n", shmadd);
    reveal_flag(shmadd);
    
    //分离共享内存和当前进程
    ret = shmdt(shmadd);
    if(ret < 0)
    {
        perror("shmdt");
        exit(1);
    }
 
    //删除共享内存
    shmctl(shmid, IPC_RMID, NULL);

    remove("./.info/lock");
    return 0;
}

int main(int argc, char **argv)
{
	if (my_ipc_child() == 0) {
		printf("Box revealed a possible flag, is it readable ? :\n");
		printf("flag{%s}\n", flag);
	}
	else
		init_room();

	return 0;
}
