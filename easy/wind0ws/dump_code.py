'''
normal code :
    Byte    : 0   1  2   3  4   5  ...
    Meaning : arg op arg op arg op ...
'''
import random

OP_NOP = 0x00
OP_SET = 0x01
OP_ADD = 0x02
OP_NEXT = 0x03
OP_PRINT = 0x04

ARG_1 = 0x01

UP_CASE_BASE = ord('A')
LOW_CASE_BASE = ord('a')

def tob(num):
    return num.to_bytes(1, byteorder='big')

def gen_op_byte(string):
    res = []
    for ch in string:
        if ch.isupper():
            delta = ord(ch) - UP_CASE_BASE
            res.append(UP_CASE_BASE)
            res.append(OP_SET)
            res.append(delta)
            res.append(OP_ADD)
            res.append(ARG_1)
            res.append(OP_NEXT)
            continue
        if ch.islower():
            delta = ord(ch) - LOW_CASE_BASE
            res.append(LOW_CASE_BASE)
            res.append(OP_SET)
            res.append(delta)
            res.append(OP_ADD)
            res.append(ARG_1)
            res.append(OP_NEXT)
            continue
        res.append(ord(ch))
        res.append(OP_SET)
        res.append(ARG_1)
        res.append(OP_NEXT)
    return res

def gen_random_keys(length):
    res = []
    for i in range(length):
        r = random.randint(0, 255)
        res.append(r)
    return res

def xor_merge_bytes(msg):
    res = []
    keys = gen_random_keys(len(msg))
    for i in range(len(msg)):
        k = keys[i]
        m = msg[i]
        res.append(k)
        res.append(k^m)
    return res

def convert_to_byte(msg):
    res = b''
    for i in msg:
        res += tob(i)
    return res

# print "Hello world!"
def gen_welcome_msg():
    res = gen_op_byte("Hellow world")
    res.append(0x00)
    res.append(OP_PRINT)
    res = xor_merge_bytes(res)
    res = convert_to_byte(res)
    print(res)
    return res

def gen_flag():
    res = gen_op_byte("flag{We11_d0ne_binary_h4cker}")
    res.append(0x00)
    res.append(OP_PRINT)
    res = xor_merge_bytes(res)
    res = convert_to_byte(res)
    print(res)
    return res

def dump(name, byts):
    with open(name, 'wb') as f:
        f.write(byts)

if __name__ == '__main__':
    dump('example.code', gen_welcome_msg())
    dump('flag.code', gen_flag())























