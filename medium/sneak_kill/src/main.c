/// flag : xxaaaaxxxdddddddwwdddxtxxk
/// or : xxaaaaxxxdddddddwwdddxtxxb

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "guard_view.c"

#define MAP_W 42
#define MAP_H 10
#define MAX_TIME 32

int TIME_STOP = 0;
int TIME_CNT = 0;

int MANA = 1;
int PLAYER_X = 17;
int PLAYER_Y = 0;

// final position of target -> (23, 6)
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define LOOP_BOUND 8
#define TRUE 1
#define FALSE 0
int TARGET_X = 23;
int TARGET_Y = 4;

unsigned char steps[LOOP_BOUND] = {
        RIGHT, RIGHT,
        DOWN, DOWN,
        LEFT, LEFT,
        UP, UP
};

char *input_cmds;

/// TODO: For Debug Only
//#define DEBUG_SNEAK
#ifdef DEBUG_SNEAK
#include "debug_sneak.h"
#endif

void get_input() {
    printf("[Carefully Sneak, And, Kill The Target]\n");

    input_cmds = (char *)malloc(sizeof(char) * MAX_TIME + 1);
    memset(input_cmds, 0, MAX_TIME + 1);
    input_cmds[MAX_TIME] = '\0';

    scanf("%32s", input_cmds);
}

int handle_cmd(char cmd) {
    switch(cmd) {
        case 'w':
            PLAYER_Y -= 1;
            break;
        case 's':
            break;
        case 'a':
            PLAYER_X -= 1;
            break;
        case 'd':
            PLAYER_X += 1;
            break;
        case 'x':
            PLAYER_Y += 1;
            break;
        case 't':
            if (MANA > 0) {
                TIME_STOP = 3;
                MANA -= 1;
            }
            break;
        case 'k':
            if ((PLAYER_Y == TARGET_Y) && (PLAYER_X == TARGET_X)) {
                printf("KILL!\n");
                printf("You grab flag from dead body : flag{%s}\n", input_cmds);
                free(input_cmds);
                exit(0);
            }
            break;
        case 'b':
            if ((PLAYER_Y == TARGET_Y) && (PLAYER_X == TARGET_X)) {
                printf("You 'borrow' the flag from target : flag{%s}\n", input_cmds);
                printf("And run away, good man\n");
                free(input_cmds);
                exit(0);
            }
        default:
            printf("You Make Some Noise\n");
            return FALSE;
    }
    return TRUE;
}

int bound_check(){
    if (PLAYER_X < 0 || PLAYER_X >= MAP_W) {
        return FALSE;
    }
    if (PLAYER_Y < 0 || PLAYER_Y >= MAP_H) {
        return FALSE;
    }
    return TRUE;
}

int guard_check(){
    unsigned int time_bit = 0x1 << TIME_CNT;
    unsigned int view_pos = PLAYER_X + PLAYER_Y * MAP_W;
    if ((guard1[view_pos] & time_bit) | (guard2[view_pos] & time_bit)) {
        return FALSE;
    }
    return TRUE;
}

void target_mv() {
    if (TIME_STOP) {
        return;
    }

    int mv = steps[TIME_CNT % LOOP_BOUND];
    switch (mv) {
        case UP:
            TARGET_Y -= 1;
            break;
        case DOWN:
            TARGET_Y += 1;
            break;
        case LEFT:
            TARGET_X -= 1;
            break;
        case RIGHT:
            TARGET_X += 1;
        default:
            return;
    }
}

int main() {
    int i = 0;
    get_input();

#ifdef DEBUG_SNEAK
    print_map();
#endif

    while (TRUE) {
        if (bound_check() == FALSE) {
            break;
        }
        if (guard_check() == FALSE) {
            break;
        }

        /// print map
#ifdef DEBUG_SNEAK
        putchar(input_cmds[i]);
        putchar('\n');
        print_map();
#endif

        if (handle_cmd(input_cmds[i]) == FALSE) {
            break;
        }
        target_mv();

        if (TIME_STOP) {
            TIME_STOP -= 1;
        }
        else {
            TIME_CNT += 1;
        }

        i += 1;
    }

    printf("Detected By Guard, You die\n");
    free(input_cmds);
    return 0;
}


