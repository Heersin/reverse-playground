void print_map() {
    unsigned int time_bit = 0x1 << TIME_CNT;
    int player_index = PLAYER_X + PLAYER_Y * MAP_W;
    int target_index = TARGET_X + TARGET_Y * MAP_W;
    int cnt = 0;
    for (int i = 0; i < MAP_W * MAP_H; ++i) {
        if (i == player_index) {
            putchar('^');
        }
        else if (i == target_index) {
            putchar('@');
        }
        else {
            putchar(((guard2[i] | guard1[i]) & time_bit) ?
                    '#':
                    '*'
            );
        }

        cnt += 1;
        if (cnt == MAP_W) {
            cnt = 0;
            putchar('\n');
        }
    }
    printf("----------------------------------------------------\n");
}