from audioop import add
from pydoc import locate

opcode_tbl = {
    'nop' : 0x00,
    'set' : 0x01,
    'and' : 0x02,
    'or' : 0x03,
    'xor' : 0x04,
    'add' : 0x05,
    'sub' : 0x06,
    'mul' : 0x07,
    'eq' : 0x08,
    'loadb' : 0x09,
    'storeb' : 0x0a,
    'mov' : 0x0b,
    'jmp' : 0x0c,
    'cjmp' : 0x0d,
    'ncjmp' : 0x0e,
    'cinc' : 0x0f,
    'call' : 0x10,
    'switch_ctx' : 0x11,
    'ret' : 0x12,
    'syscall' : 0x13,
    'setds' : 0x14,
    'setcs' : 0x15,
    'exit' : 0x16,
    'getret' : 0x17,
    'junk' : 0x18,
}

with open('./real.asm') as f:
    lines = f.readlines()

instructions = []

for line in lines:
    if line.strip() == '':
        continue
    if '#' in line:
        continue
    instructions.append(line.strip())

def token2reg(token):
    reg_token = token[1:]
    reg_num = int(reg_token)
    return reg_num

def token2num(token):
    if '0x' in token:
        return int(token, base=16)
    else:
        return int(token)

def token2name(token):
    return token[1:]

def name2addr(tbl, name):
    offset = 0
    if '+' in name:
        tks = name.split('+')
        name = tks[0]
        offset = token2num(tks[1])

    if name in tbl:
        return tbl[name] + offset
    else:
        print("[Fatal Error], location not found : " + name)
        exit(0)

def reg2byte(reg):
    return reg

def num2byte(num):
    a = (num & 0xff00) >> 8
    b = num & 0x00ff
    return b, a

def addr2byte(addr):
    return num2byte(addr)

def op2byte(op):
    return opcode_tbl[op]

def scan_offset():
    loc_table = {}
    offset = 0

    for ins in instructions:
        if ('! $' in ins) or ('! @' in ins):
            loc_name = ins[3:]
            print("***** Building Offset **** " + loc_name + "-" + hex(offset))
            loc_table[loc_name] = offset
            continue

        tokens = ins.replace(' ', '').split(',')
        op = tokens[0]
        print(op, end='')
        if op == 'set':
            # 1B
            reg = token2reg(tokens[1])
            # 2B (16-bit)
            num = token2num(tokens[2])
            offset += 1 + 1 + 2
            print(" r{}, {}".format(reg, num))
            pass
        
        elif op == 'and' or op == 'or' or  op == 'xor' or op == 'add' or op == 'sub' or op == 'mul' or op == 'eq':
            # 1B
            dst = token2reg(tokens[1])
            reg = token2reg(tokens[2])
            offset += 1 + 1 + 1
            print(" r{}, r{}".format(dst, reg))
            pass
        
        elif op == 'cinc':
            reg = token2reg(tokens[1])
            offset += 1 + 1
            print(" r{}".format(reg))
            pass

        elif op == 'loadb' or op == 'storeb':
            lr = token2reg(tokens[1])
            rr = token2reg(tokens[2])
            offset += 1 + 1 + 1
            print(" r{}, r{}".format(lr, rr))
            pass

        elif op == 'mov':
            dst = token2reg(tokens[1])
            reg = token2reg(tokens[2])
            offset += 1 + 1 + 1
            print(" r{} r{}".format(dst, reg))
            pass

        elif op == 'jmp' or op == 'cjmp' or op == 'ncjmp':
            dst_addr = token2name(tokens[1])
            offset += 1 + 2
            print(" {}".format(dst_addr))
            pass

        elif op == 'call':
            dst_addr = token2name(tokens[1])
            offset += 1 + 2
            print(" {}".format(dst_addr))
            pass

        elif op == 'syscall':
            call_num = token2num(tokens[1])
            offset += 1 + 1
            print(" {}".format(call_num))
            pass        
            
        elif op == 'nop' or op == 'exit' or op == 'setds' or op == 'setcs' or op == 'switch_ctx' or op == 'ret' or op == 'junk' or op == 'getret':
            offset += 1
            print("")
            pass

        else:
            print("")
            print("**************************")
            pass
    print("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
    return loc_table

def build_bytes(loc_table):
    code_bytes = []
    for ins in instructions:
        if ('! $' in ins) or ('! @' in ins):
            continue

        tokens = ins.replace(' ', '').split(',')
        op = tokens[0]
        if op == 'set':
            # 1B
            reg = token2reg(tokens[1])
            # 2B (16-bit)
            num = token2num(tokens[2])            
            code_bytes.append(op2byte(op))
            code_bytes.append(reg2byte(reg))
            a, b = num2byte(num)
            code_bytes.append(a)
            code_bytes.append(b)
        
        elif op == 'and' or op == 'or' or  op == 'xor' or op == 'add' or op == 'sub' or op == 'mul' or op == 'eq':
            # 1B
            dst = token2reg(tokens[1])
            reg = token2reg(tokens[2])
            code_bytes.append(op2byte(op))
            code_bytes.append(reg2byte(dst))
            code_bytes.append(reg2byte(reg))
        
        elif op == 'cinc':
            reg = token2reg(tokens[1])
            code_bytes.append(op2byte(op))
            code_bytes.append(reg2byte(reg))

        elif op == 'loadb' or op == 'storeb' or op == 'mov':
            lr = token2reg(tokens[1])
            rr = token2reg(tokens[2])
            code_bytes.append(op2byte(op))
            code_bytes.append(reg2byte(lr))
            code_bytes.append(reg2byte(rr))
            pass

        elif op == 'jmp' or op == 'cjmp' or op == 'ncjmp':
            dst_addr = name2addr(loc_table, token2name(tokens[1]))
            a, b = addr2byte(dst_addr)
            code_bytes.append(op2byte(op))
            code_bytes.append(a)
            code_bytes.append(b)
            

        elif op == 'call':
            dst_addr = name2addr(loc_table, token2name(tokens[1]))
            a, b = addr2byte(dst_addr)
            code_bytes.append(op2byte(op))
            code_bytes.append(a)
            code_bytes.append(b)

        elif op == 'syscall':
            call_num = token2num(tokens[1])
            code_bytes.append(op2byte(op))
            code_bytes.append(call_num)
            
        elif op == 'nop' or op == 'exit' or op == 'setds' or op == 'setcs' or op == 'switch_ctx' or op == 'ret' or op == 'junk' or op == 'getret':
            code_bytes.append(op2byte(op))

        else:
            print(op)
            print("**************************")
            pass

    return code_bytes

loctbl = scan_offset()
code_hex = build_bytes(loctbl)

print("{ ", end='')
for i in code_hex:
    print(hex(i) + ', ', end='')
print(" };")
