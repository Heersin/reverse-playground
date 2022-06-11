#include "common.h"
#include "mem_inc.c"

/// define as global static
handler op_handlers[OP_NUMS];
VM main_vm = {
        .pc = 0
};

bool vm_init(VM *vm) {
    if (!vm) {
        return FALSE;
    }

    /// set regs
    memset(vm->main_regs, 0x0, REG_NUM * REG_SZ);
    memset(vm->sub_regs, 0x0, REG_NUM * REG_SZ);
    vm->regs = (ut16 *)vm->main_regs;

    /// TODO
    vm->mem_slot[MEM_CS] = cs_mem;
    vm->mem_slot[MEM_DS] = ds_mem;
    vm->mem_idx = MEM_DS;
    ds_mem[0] = 'p';
    ds_mem[1] = 'l';
    ds_mem[2] = 'z';
    ds_mem[3] = ',';
    ds_mem[4] = 's';
    ds_mem[5] = 'p';
    ds_mem[6] = 'e';
    ds_mem[7] = 'l';
    ds_mem[8] = 'l';
    ds_mem[9] = ',';
    ds_mem[10] = 't';
    ds_mem[11] = 'o';
    ds_mem[12] = ' ';
    ds_mem[13] = 'e';
    ds_mem[14] = 'n';
    ds_mem[15] = 't';
    ds_mem[16] = 'e';
    ds_mem[17] = 'r';
    ds_mem[18] = '\n';
    ds_mem[19] = 0;

    vm->ret_value = 0;
    vm->ret_addr = 0;

    return TRUE;
}

void table_init(handler *tbl) {
    tbl[0] = handle_nop;
    tbl[1] = handle_set;
    tbl[2] = handle_and;
    tbl[3] = handle_or;
    tbl[4] = handle_xor;
    tbl[5] = handle_add;
    tbl[6] = handle_sub;
    tbl[7] = handle_mul;
    tbl[8] = handle_eq;
    tbl[9] = handle_loadb;
    tbl[10] = handle_storeb;
    tbl[11] = handle_mov;
    tbl[12] = handle_jmp;
    tbl[13] = handle_cjmp;
    tbl[14] = handle_ncjmp;
    tbl[15] = handle_cinc;
    tbl[16] = handle_call;
    tbl[17] = handle_switch_ctx;
    tbl[18] = handle_ret;
    tbl[19] = handle_syscall;
    tbl[20] = handle_setds;
    tbl[21] = handle_setcs;
    tbl[22] = handle_exit;
    tbl[23] = handle_getret;
    tbl[24] = handle_junk;
    tbl[25] = NULL;
}

int main() {
    /// init vm
    if (!vm_init(&main_vm)) {
        printf("Crashed\n");
        return 1;
    }

    /// init op handlers
    table_init(op_handlers);

    /// run
    byte op_byte = cs_mem[0];
    while (op_byte < OP_VALID_BOUND) {
        handler worker;
        if (op_byte < OP_JUNK_START) {
            worker = op_handlers[op_byte];
        }
        else {
            /// Never Execute
            worker = op_handlers[OP_JUNK_START];
        }

        STATUS status = worker(&main_vm);
        if (status == Err) {
            return 1;
        }
        if (status == Halt) {
            return 0;
        }
        op_byte = cs_mem[main_vm.pc];
    }

    puts("ByeBye\n");
    return 0;
}
