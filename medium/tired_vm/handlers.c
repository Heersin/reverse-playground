#include "common.h"

static ut16 pack_num(byte A, byte B) {
    ut16 num = 0;
    ut16 A_ext = A;
    ut16 B_ext = B;
    num = A_ext | (B_ext << 8);
    return num;
}

STATUS handle_nop(VM *vm) {
    ut16 offset = 1;
    vm->pc += offset;
    return OK;
}

STATUS handle_set(VM *vm) {
    ut16 offset = 1 + 1 + 2;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;

    byte reg_num = code[pc + 1];
    ut16 instant_num = pack_num(code[pc + 2], code[pc + 3]);

    if (reg_num > REG_NUM - 1) {
        return Err;
    }
    vm->regs[reg_num] = instant_num;
    vm->pc += offset;
    return OK;
}

STATUS handle_and(VM *vm) {
    ut16 offset = 1 + 1 + 1;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;

    byte reg_a = code[pc + 1];
    byte reg_b = code[pc + 2];
    vm->regs[reg_a] = vm->regs[reg_a] & vm->regs[reg_b];

    vm->pc += offset;
    return OK;
}

STATUS handle_or(VM *vm) {
    ut16 offset = 1 + 1 + 1;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;

    byte reg_a = code[pc + 1];
    byte reg_b = code[pc + 2];
    vm->regs[reg_a] = vm->regs[reg_a] | vm->regs[reg_b];

    vm->pc += offset;
    return OK;
}

STATUS handle_xor(VM *vm) {
    ut16 offset = 1 + 1 + 1;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;

    byte reg_a = code[pc + 1];
    byte reg_b = code[pc + 2];
    vm->regs[reg_a] = vm->regs[reg_a] ^ vm->regs[reg_b];

    vm->pc += offset;
    return OK;
}

STATUS handle_add(VM *vm) {
    ut16 offset = 1 + 1 + 1;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;

    byte reg_a = code[pc + 1];
    byte reg_b = code[pc + 2];
    vm->regs[reg_a] = vm->regs[reg_a] + vm->regs[reg_b];

    vm->pc += offset;
    return OK;
}

STATUS handle_sub(VM *vm) {
    ut16 offset = 1 + 1 + 1;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;

    byte reg_a = code[pc + 1];
    byte reg_b = code[pc + 2];
    vm->regs[reg_a] = vm->regs[reg_a] - vm->regs[reg_b];

    vm->pc += offset;
    return OK;
}

STATUS handle_mul(VM *vm) {
    ut16 offset = 1 + 1 + 1;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;

    byte reg_a = code[pc + 1];
    byte reg_b = code[pc + 2];
    vm->regs[reg_a] = vm->regs[reg_a] * vm->regs[reg_b];

    vm->pc += offset;
    return OK;
}

STATUS handle_eq(VM *vm) {
    ut16 offset = 1 + 1 + 1;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;

    byte reg_a = code[pc + 1];
    byte reg_b = code[pc + 2];
    vm->condition_flag = (vm->regs[reg_a] == vm->regs[reg_b]);

    vm->pc += offset;
    return OK;
}

STATUS handle_loadb(VM *vm) {
    ut16 offset = 1 + 1 + 1;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;

    byte reg_a = code[pc + 1];
    byte reg_b = code[pc + 2];
    byte *mem = vm->mem_slot[vm->mem_idx];
    vm->regs[reg_a] = mem[vm->regs[reg_b]];

    vm->pc += offset;
    return OK;
}

STATUS handle_storeb(VM *vm) {
    ut16 offset = 1 + 1 + 1;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;

    byte reg_a = code[pc + 1];
    byte reg_b = code[pc + 2];
    byte *mem = vm->mem_slot[vm->mem_idx];
    vm->regs[reg_a] = mem[vm->regs[reg_b]];

    vm->pc += offset;
    return OK;
}

STATUS handle_mov(VM *vm) {
    ut16 offset = 1 + 1 + 1;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;

    byte reg_a = code[pc + 1];
    byte reg_b = code[pc + 2];
    vm->regs[reg_a] = vm->regs[reg_b];

    vm->pc += offset;
    return OK;
}

STATUS handle_jmp(VM *vm) {
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;
    ut16 jump_target = pack_num(code[pc + 1], code[pc + 2]);
    vm->pc = jump_target;
    return OK;
}

STATUS handle_cjmp(VM *vm) {
    ut16 offset = 1 + 2;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;
    ut16 jump_target = pack_num(code[pc + 1], code[pc + 2]);

    if (vm->condition_flag == TRUE) {
        vm->pc = jump_target;
    }
    else {
        vm->pc += offset;
    }
    return OK;
}

STATUS handle_ncjmp(VM *vm) {
    ut16 offset = 1 + 2;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;
    ut16 jump_target = pack_num(code[pc + 1], code[pc + 2]);

    if (vm->condition_flag == FALSE) {
        vm->pc = jump_target;
    }
    else {
        vm->pc += offset;
    }
    return OK;
}

STATUS handle_cinc(VM *vm) {
    ut16 offset = 1 + 1;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;

    byte reg_num = code[pc + 1];
    if (vm->condition_flag == TRUE) {
        vm->regs[reg_num] += 1;
    }

    vm->pc += offset;
    return OK;
}

STATUS handle_call(VM *vm) {
    ut16 offset = 1 + 2;
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;

    vm->ret_addr = vm->pc + offset;
    ut16 jump_target = pack_num(code[pc + 1], code[pc + 2]);
    vm->pc = jump_target;
    return OK;
}

STATUS handle_switch_ctx(VM *vm) {
    ut16 offset = 1;

    if (vm->regs == vm->main_regs) {
        vm->regs = vm->sub_regs;
    }
    else {
        vm->regs = vm->main_regs;
    }

    vm->pc += offset;
    return OK;
}

STATUS handle_ret(VM *vm) {
    vm->ret_value = vm->regs[0];
    vm->pc = vm->ret_addr;
    return OK;
}

STATUS handle_getret(VM *vm) {
    vm->regs[0] = vm->ret_value;
    vm->pc += 1;
    return OK;
}

STATUS handle_setds(VM *vm) {
    vm->mem_idx = MEM_DS;
    vm->pc += 1;
    return OK;
}

STATUS handle_setcs(VM *vm) {
    vm->mem_idx = MEM_CS;
    vm->pc += 1;
    return OK;
}

STATUS handle_exit(VM *vm) {
    return Halt;
}

STATUS handle_syscall(VM *vm) {
    byte *code = vm->mem_slot[MEM_CS];
    ut16 pc = vm->pc;
    ut16 offset = 1 + 1;

    byte sys_num = code[pc + 1];
    ut16 r0 = vm->regs[0];
    STATUS status = Err;

    switch (sys_num) {
        case 0:
            status = syscall_0_input(vm, r0);
            break;
        case 1:
            status = syscall_1_print(vm, r0);
            break;
        case 2:
            status = syscall_2_length(vm, r0);
            break;
        case 3:
            status = syscall_3_nice(vm);
            break;
        case 4:
            status = syscall_4_fail(vm);
            break;
        default:
            return Err;
    }

    vm->pc += offset;
    return status;
}

/// Junk Bytes
/// Warn If Reached
STATUS handle_junk(VM *vm) {
    byte *code = vm->mem_slot[MEM_CS];

    byte op_byte = code[vm->pc + 1];
    /// junk byte can be : 24 - 124
    byte base = op_byte - 24;
    float raw = base * 1.0f;

    /// newton sqrt
    float x1, x2;
    x2 = 1.0f;
    do
    {
        x1 = x2;
        x2 = (x1 + raw / x1) / 2;
    } while (x1 - x2 > 10E-5);
    int result = (int)x2 * 10;

    /// write code mem
    code[result] = base;
    vm->pc += 1;

    return OK;
}
