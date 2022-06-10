import sys

def get_byte_len(filename):
    f = open(filename,'rb')
    byt = f.read()
    f.close()
    return len(byt)

def append_byte(filename, num):
    with open(filename, mode='ab') as f:
        byts = b'\x00' * num
        f.write(byts)

if __name__ == '__main__':
    target_len = 0
    filename = ""
    try:
        filename = sys.argv[1]
        target_len = int(sys.argv[2])
    except:
        print("Usage: target_append.py [filename] [target_len]")
        exit(1)

    origin_len = get_byte_len(filename)
    if (origin_len >= target_len):
        print("Cannot Append")
        exit(1)

    delta = target_len - origin_len
    append_byte(filename, delta)

