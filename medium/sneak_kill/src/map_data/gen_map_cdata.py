import map_show


def get_map_size(map_snapshot):
    y = len(map_snapshot)
    x = len(map_snapshot[0])
    return x, y


def encode_map(m):
    max_t = map_show.get_max_time(m)
    x, y = get_map_size(map_show.get_map_by_time(m, 0))
    map_bitmap = [0] * x * y

    for i in range(max_t + 1):
        map_snapshot = map_show.get_map_by_time(m, i)
        cnt = 0
        time_bit = 0x1 << i
        for row in map_snapshot:
            for mark in row:
                if mark == '#':
                    map_bitmap[cnt] += time_bit
                elif mark == '*':
                    pass
                else:
                    print("[Error] Mark not # or *")
                cnt += 1

    return map_bitmap


def decode_map(encode_map, bits, w, h):
    ret_map = {}
    for t in range(bits):
        time_bit = 0x1 << t
        snapshot = [[0] * w] * h
        print(len(encode_map))
        for i in range(len(encode_map)):
            num = encode_map[i]
            x = i % w
            y = i // w
            print((x, y))
            if num & time_bit == 0x1:
                snapshot[y][x] = '#'
            else:
                snapshot[y][x] = '*'
        ret_map[t] = snapshot
        map_show.print_map(snapshot)
    return ret_map


def print_encode_map(encode_map):
    cnt = 0
    for i in encode_map:
        print("0x%08X, " % i, end='')
        cnt += 1
        if cnt == map_show.MAP_WIDTH:
            print("")
            cnt = 0

    print("")


guard1 = map_show.read_map('./guard1.txt')
guard2 = map_show.read_map('./guard2.txt')
#decode_map(encode_map(guard1), 1, 42, 10)
print_encode_map(encode_map(guard1))
print_encode_map(encode_map(guard2))