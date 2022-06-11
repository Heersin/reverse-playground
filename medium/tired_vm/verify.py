from z3 import *

def cnt_odd(string):
    res = 0
    for i in string:
        print(ord(i))
        if ord(i) & 0x01 == 0:
            res += 1

    return res

def print_m(var_list, m):
    for i in var_list:
        print(i, end='')
        print(" ", end='')
        print(chr(m[i].as_long()))

def except_solution(var_list, m):
    condition = []
    for i in var_list:
        condition.append(i != m[i].as_long())
        solver.add(Or(condition))
    assert(solver.check() == unsat)
    print("No More Solution")

# ==============================
print("============Left===========")
solver = z3.Solver()

emu_flag = []
for i in range(14):
    emu_flag.append(BitVec("flag%d" % i, 16))

# count odd
cons = 0
for i in emu_flag:
    cons += (i & 0x01)
solver.add(cons == 5)

# string constraints
solver.add(emu_flag[1] == ord('U'))
solver.add(emu_flag[0] ^ emu_flag[1] == 116)
solver.add(emu_flag[13] == ord('g'))
solver.add(emu_flag[4] | emu_flag[4] == ord('3'))
#
solver.add(emu_flag[2] == emu_flag[9])
solver.add(emu_flag[2] == emu_flag[12])
solver.add(emu_flag[2] == ord('n'))
#
solver.add(emu_flag[3] == emu_flag[10])
solver.add(emu_flag[3] ^ emu_flag[6] == emu_flag[8])
#
solver.add(emu_flag[11] * emu_flag[8] == 3136)
solver.add(emu_flag[7] - emu_flag[5] == 2)
solver.add(emu_flag[8] + emu_flag[10] == 164)
solver.add(emu_flag[5] == ord('r'))

# ascii printable
for i in emu_flag:
    solver.add(i >= 32)
    solver.add(i <= 126)

print(solver.check())
result = solver.model()
print_m(emu_flag, result)
except_solution(emu_flag, result)

# right part
# 后半部分位简单矩阵(方程组)求解
print ("======== Right ========")
solver.reset()
flag_right = [BitVec("x%d" % i, 16) for i in range(9)]
res = 0
accs = [86, 163, 258, 308, 403, 511, 629, 746, 858]
for i in range(len(accs)):
    res += flag_right[i]
    solver.add(res == accs[i])

print(solver.check())
print_m(flag_right, solver.model())
except_solution(flag_right, solver.model())
