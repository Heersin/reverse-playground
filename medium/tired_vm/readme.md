# tired_VM
## 项目结构
```
├── assmbler.py ： 汇编器， 将real.asm转成字节
├── build.sh ： 二进制文件构建脚本
├── CMakeLists.txt ： CMake脚本
├── common.h ： project header
├── handlers.c ： 指令处理的实现
├── main.c ： VM入口
├── mem_inc.c ： 指令数据
├── readme.md
├── real.asm ： 实际的汇编代码
├── tired_vm ： 题目二进制文件
├── verify.py ： flag的约束求解脚本
└── vm_syscall.c ： VM中syscal的实现
```

### 二进制文件构建
> cd tired_vm
> ./build.sh

### FLAG
> flag{!Und3r$t@nd1ng_VM_2_lvup}

## 考察点
1. VM程序的识别，本题用函数指针实现指令的解释执行
2. 结构体信息的复原
3. 基本的VM指令逆向（算术指令、逻辑指令、控制指令等）
4. 函数调用识别
5. 能够编写简单反汇编器及求解脚本
6. 阅读汇编代码的能力，能识别常见的干扰指令序列,如
    - xor r0, r0; eq r0, 0; jz xxx; [junk bytes]
    - jz xxx; jnz xxx; [junk bytes]

## VM 信息
### 结构
```C
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
```
1. 通用寄存器寄存器大小都是16-bit, 共两组，每组32个寄存器（r0-r31）, 在主程序中都是用`main_regs`， 仅当进入函数时使用`sub_regs`，`regs`用于指示当前寄存器组，指令集中提供了`switch_ctx`用于切换当前寄存器组
2. 内存总共有两块，分别用来放代码和数据，可以根据`vm->mem_slot[vm->mem_idx]`获取当前正在使用的内存，也可以用宏指定使用代码/数据段。指令集中提供`setds`和`setcs`将当前内存切换数据内存/代码内存
3. `condition_flag`寄存器用于条件判断，1代表条件为真，0代表条件为假
4. `ret_addr` 记录了函数返回地址
5. `ret_value` 记录了函数返回值，可以用`getret`将返回值复制到当前寄存器的r0中
6. `pc`程序计数器

### 指令
- num大小为2B
- reg大小为1B
- bnum大小为1B

0. 0x00 -- nop
    - 无操作

1. 0x01 -- set
    - set [reg] [num]
    - 将reg设置为立即数

2. 0x02 -- and
    - and [reg1] [reg2]
    - reg1 &= reg2

3. 0x03 -- or
    - or [reg1] [reg2]
    - reg1 |= reg2

4. 0x04 -- xor
    - xor [reg1] [reg2]
    - reg1 ^= reg2

5. 0x05 -- add
    - add [reg1] [reg2]
    - reg1 += reg2

6. 0x06 -- sub
    - sub [reg1] [reg2]
    - reg1 -= reg2

7. 0x07 -- mul
    - mul [reg1] [reg2]
    - reg1 *= reg2

8. 0x08 -- eq
    - eq [reg1] [reg2]
    - condigtion_flag = (reg1 == reg2)

9. 0x09 -- loadb, 加载一个Byte
    - loadb [reg1] [reg2]
    - reg1 = MEM[reg2]

10. 0x0a -- storeb 存入一个Byte
    - storeb [reg1] [reg2]
    - MEM[reg2] = reg1

11. 0x0b -- mov
    - mov [reg1] [reg2]
    - reg1 = reg2

12. 0x0c -- jmp
    - jmp [num]
    - pc = num

13. 0x0d -- cjmp
    - cjmp [num]
    - if (coditional_flag) pc = num

14. 0x0e -- ncjmp
    - cjmp [num]
    - if (!conditional_flag) pc = num

15. 0x0f -- cinc
    - cinc [reg]
    - if (conditional_flag) reg += 1

16. 0x10 -- call
    - call [num]
    - 函数调用

17. 0x11 -- switch_ctx
    - switch_ctx
    - 切换环境

18. 0x12 -- ret
    - ret
    - pc = ret_addr

19. 0x13 -- syscall
    - syscall [bnum]

20. 0x14 -- setds
    - setds
    - 切换为Data Mem

21. 0x15 -- setcs
    - setcs
    - 切换为Code Mem

22. 0x16 -- exit
    - exit
    - 退出

23. 0x17 -- getret
    - getret
    - r0 = ret_value

24. 0x18 -- junk
    - junk
    - 无用的干扰指令

### 函数调用
例子
```
switch_ctx
set, r0, 0x100
call, $length
switch_ctx
getret
```
1. 首先用`switch_ctx`切换寄存器环境
2. 准备参数，接着调用函数`length`
3. `switch_ctx`切换寄存器环境
4. `getret`将返回值放入r0

从设计上看得出来，VM只支持一层的函数调用

### syscall
0. input r0, 获取输入，r0存放输入字符串的地址 
1. print r0, 打印，r0存放字符串地址
2. length r0, 统计长度，r0为字符串地址，返回值（字符串长度）放在r0中
3. nice, 打印成功信息和flag
4. fail, 打印失败信息

## 原始汇编代码
由于手动把循环展开了所以比较长，在`real.asm`中

## flag 约束及求解脚本
> flag = "!Und3r$t@nd1ng_VM_2_lvup"
```py
# flag = "!Und3r$t@nd1ng_VM_2_lvup"
# 长度为24

# 前半部分flag的约束
flag_left = [BitVec("y%d" % i, 16) for i in range(13)]

# 9个偶数
cons = 0
for i in flag_left:
    cons += (i & 0x01)
solver.add(cons == 5)

# 字符串约束
solver.add(flag_left[1] == ord('U'))
solver.add(flag_left[0] ^ flag_left[1] == 116)
solver.add(flag_left[13] == ord('g'))
solver.add(flag_left[4] | flag_left[4] == ord('3'))
solver.add(flag_left[2] == flag_left[9])
solver.add(flag_left[2] == flag_left[12])
solver.add(flag_left[2] == ord('n'))
solver.add(flag_left[3] == flag_left[10])
solver.add(flag_left[3] ^ flag_left[6] == flag_left[8])
solver.add(flag_left[11] * flag_left[8] == 3136)
solver.add(flag_left[7] - flag_left[5] == 2)
solver.add(flag_left[8] + flag_left[10] == 164)
solver.add(flag_left[5] == ord('r'))

# 都是可打印字符
for i in flag_left:
    solver.add(i >= 32)
    solver.add(i <= 126)

# 分隔符
flag[14] == '_'

# 后半部分位简单矩阵(方程组)求解
flag_right = [BitVec("x%d" % i, 16) for i in range(9)]

res = 0
accs = [86, 163, 258, 308, 403, 511, 629, 746, 858]
for i in range(len(accs)):
    res += flag_right[i]
    solver.add(res == accs[i])

```
