#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef YI2JIA_COMMON_H
#define YI2JIA_COMMON_H
typedef unsigned short ut16;
typedef unsigned char byte;
typedef unsigned char bool;

#define TRUE 1
#define FALSE 0
#define MEM_CS 0x0
#define MEM_DS 0x1
#define REG_NUM 32
#define REG_SZ 2
#define MEM_SLOT_NUM 2
#define OP_NUMS 26
#define OP_VALID_BOUND 125
#define OP_JUNK_START 24

typedef struct vm_t VM;
typedef enum status_enum STATUS;
typedef STATUS(*handler)(VM *vm);

struct vm_t {
    ut16 main_regs[REG_NUM];
    ut16 sub_regs[REG_NUM];
    ut16 *regs;

    ut16 ret_value;
    ut16 ret_addr;
    ut16 pc;
    ut16 condition_flag;

    ut16 mem_idx;
    byte *mem_slot[MEM_SLOT_NUM];
};

enum status_enum {
    OK = 0,
    Err = 1,
    Halt = 2
};

STATUS syscall_0_input(VM *vm, ut16 addr);
STATUS syscall_1_print(VM *vm, ut16 addr);
STATUS syscall_2_length(VM *vm, ut16 str_addr);
STATUS syscall_3_nice(VM *vm);
STATUS syscall_4_fail(VM *vm);

STATUS handle_nop(VM *vm);
STATUS handle_set(VM *vm);
STATUS handle_and(VM *vm);
STATUS handle_or(VM *vm);
STATUS handle_xor(VM *vm);
STATUS handle_add(VM *vm);
STATUS handle_sub(VM *vm);
STATUS handle_mul(VM *vm);
STATUS handle_eq(VM *vm);
STATUS handle_loadb(VM *vm);
STATUS handle_storeb(VM *vm);
STATUS handle_mov(VM *vm);
STATUS handle_jmp(VM *vm);
STATUS handle_cjmp(VM *vm);
STATUS handle_ncjmp(VM *vm);
STATUS handle_cinc(VM *vm);
STATUS handle_call(VM *vm);
STATUS handle_switch_ctx(VM *vm);
STATUS handle_ret(VM *vm);
STATUS handle_getret(VM *vm);
STATUS handle_setds(VM *vm);
STATUS handle_setcs(VM *vm);
STATUS handle_exit(VM *vm);
STATUS handle_syscall(VM *vm);
STATUS handle_junk(VM *vm);

#endif //YI2JIA_COMMON_H
