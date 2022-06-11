setds

# load and print addr of "plz, spell, to enter"
set, r0, 0x0
syscall, 1

# read user input, and sotre at Data[0x100]
set, r0, 0x100
syscall, 0

# call len to calculate input length
switch_ctx
set, r0, 0x100
call, $length
switch_ctx

# check len
getret
set, r1, 24
eq, r0, r1
cjmp, $check_success

! $check_fail
switch_ctx
call, @fail
switch_ctx

! $check_success
# anti-disasm-trick
xor, r0, r0
set, r1, 0
eq, r0, r1
cjmp, $correct1

# junk bytes
setcs
set, r2, 0x0
set, r3, 0x1
add, r2, r3
loadb, r1, r2
set, r4, 0x42
xor, r1, r4
storeb, r1, r2
junk

! $correct1
# anti-disasm-trick
eq, r0, r1
cjmp, $correct2
ncjmp, $correct2
jmp, $correct2 + 1

! $correct2
# load string into regs
set, r30, 0x100
set, r29, 1
loadb, r0, r30
add, r30, r29
loadb, r1, r30
add, r30, r29
loadb, r2, r30
add, r30, r29
loadb, r3, r30
add, r30, r29
loadb, r4, r30
add, r30, r29
loadb, r5, r30
add, r30, r29
loadb, r6, r30
add, r30, r29
loadb, r7, r30
add, r30, r29
loadb, r8, r30
add, r30, r29
loadb, r9, r30
add, r30, r29
loadb, r10, r30
add, r30, r29
loadb, r11, r30
add, r30, r29
loadb, r12, r30
add, r30, r29
loadb, r13, r30
add, r30, r29
loadb, r14, r30

# set result
set, r31, 0

# check left part
# 1. flag[1] == 'U'
set, r30, 85
eq, r30, r1
cinc, r31

# 2. flag[0] ^ flag[1] == 116
set, r30, 116
mov, r29, r0
xor, r29, r1
eq, r29, r30
cinc, r31

# 3. flag[13] == 'g'
set, r30, 103
eq, r30, r13
cinc, r31

# 4. flag[4] | flag[4] == '3'
set, r30, 51
mov, r29, r4
or, r29, r29
eq, r29, r30
cinc, r31

# 5. flag[2] == flag[9]
eq, r2, r9
cinc, r31

# 6. flag[2] == flag[12]
eq, r2, r12
cinc, r31

# 7. flag[2] == 'n'
set, r30, 110
eq, r30, r2
cinc, r31

# 8. flag[3] == flag[10]
eq, r3, r10
cinc, r31

# 9. flag[3] ^ flag[6] == flag[8]
mov, r30, r3
mov, r29, r6
xor, r30, r29
eq, r30, r8
cinc, r31

# 10. flag[11] * flag[8] == 3136
mov, r30, r11
mov, r29, r8
mul, r30, r29
set, r28, 3136
eq, r30, r28
cinc, r31

# 11. flag[7] - flag[5] == 2
mov, r30, r7
set, r29, 2
sub, r30, r5
eq, r30, r29
cinc, r31

# 12. flag[8] + flag[10] == 164
mov, r30, r8
set, r29, 164
add, r30, r10
eq, r30, r29
cinc, r31

# 13. flag[5] == 'r'
set, r30, 114
eq, r30, r5
cinc, r31

# 14. 5 odd number
set, r30, 0
set, r29, 1

mov, r28, r0
and, r28, r29
add, r30, r28

mov, r28, r1
and, r28, r29
add, r30, r28

mov, r28, r2
and, r28, r29
add, r30, r28

mov, r28, r3
and, r28, r29
add, r30, r28

mov, r28, r4
and, r28, r29
add, r30, r28

mov, r28, r5
and, r28, r29
add, r30, r28

mov, r28, r6
and, r28, r29
add, r30, r28

mov, r28, r7
and, r28, r29
add, r30, r28

mov, r28, r8
and, r28, r29
add, r30, r28

mov, r28, r9
and, r28, r29
add, r30, r28

mov, r28, r10
and, r28, r29
add, r30, r28

mov, r28, r11
and, r28, r29
add, r30, r28

mov, r28, r12
and, r28, r29
add, r30, r28

mov, r28, r13
and, r28, r29
add, r30, r28

set, r29, 5
eq, r30, r29
cinc, r31

# check medium
# check flag[14] == '_'
set, r30, 95
eq, r30, r14
cinc, r31

# load right part
set, r30, 0x10f
set, r29, 1
loadb, r0, r30
add, r30, r29
loadb, r1, r30
add, r30, r29
loadb, r2, r30
add, r30, r29
loadb, r3, r30
add, r30, r29
loadb, r4, r30
add, r30, r29
loadb, r5, r30
add, r30, r29
loadb, r6, r30
add, r30, r29
loadb, r7, r30
add, r30, r29
loadb, r8, r30
add, r30, r29

# check right part
set, r30, 0

set, r29, 86
add, r30, r0
eq, r30, r29
cinc, r31

set, r29, 163
add, r30, r1
eq, r30, r29
cinc, r31

set, r29, 258
add, r30, r2
eq, r30, r29
cinc, r31

set, r29, 308
add, r30, r3
eq, r30, r29
cinc, r31

set, r29, 403
add, r30, r4
eq, r30, r29
cinc, r31

set, r29, 511
add, r30, r5
eq, r30, r29
cinc, r31

set, r29, 629
add, r30, r6
eq, r30, r29
cinc, r31

set, r29, 746
add, r30, r7
eq, r30, r29
cinc, r31

set, r29, 858
add, r30, r8
eq, r30, r29
cinc, r31

# real check
set, r30, 24
eq, r30, r31
cjmp, $nice_branch

# fail_branch
# print ("Don't give up")
call, @fail
exit

! $nice_branch
call, @nice
exit

! @nice
syscall, 3
ret

! @fail
syscall, 4
ret

! @length
syscall, 2
ret
