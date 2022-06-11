#include "common.h"

STATUS syscall_0_input(VM *vm, ut16 addr) {
    byte *data_mem = vm->mem_slot[vm->mem_idx];
    scanf("%s", &data_mem[addr]);
    return OK;
}

STATUS syscall_1_print(VM *vm, ut16 addr) {
    byte *data_mem = vm->mem_slot[vm->mem_idx];
    printf("%s", &data_mem[addr]);
    return OK;
}

STATUS syscall_2_length(VM *vm, ut16 str_addr) {
    byte *data_mem = vm->mem_slot[vm->mem_idx];
    ut16 len = strlen((char *)(data_mem + str_addr));
    vm->regs[0] = len;
    return OK;
}

STATUS syscall_3_nice(VM *vm) {
    puts("Congrats\n");
    char *flag = (char *)(&vm->mem_slot[vm->mem_idx][0x100]);
    printf("flag is flag{%s}\n", flag);
    return OK;
}

STATUS syscall_4_fail(VM *vm) {
    printf("Don't give up\n");
    return Err;
}