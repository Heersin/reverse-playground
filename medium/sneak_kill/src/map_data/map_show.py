MAP_WIDTH = 42
MAP_HEIGHT = 10
TARGET_X = 23
TARGET_Y = 4
START_X = 17
START_Y = 0
TIME_CNT = 0
STOP_TIME = 0
input_cmds = []
# final TARGET (23, 6)
target_step = ['d', 'd', 'x', 'x', 'a', 'a', 'w', 'w']


def show_map_template(h, w):
    for i in range(h):
        for i in range(w):
            print("#", end="")
        print("")


def parse_time_slice(time_slice_str : str):
    start_end = time_slice_str.split('-')
    start = int(start_end[0])
    end = int(start_end[1])
    return range(start, end + 1)


def read_map(path):
    map_dict = {}
    time_slice = "default"
    with open(path) as f:
        for row in enumerate(f):
            content = row[1].strip('\n')
            if '$' in content:
                time_slice = content[1:]
                map_dict[time_slice] = []
            else:
                content = [ch for ch in content]
                map_dict[time_slice].append(content)
    return map_dict


def get_max_time(maps):
    max_end = 0
    for t_range_str in maps:
        end = int(t_range_str.split('-')[1])
        if end > max_end:
            max_end = end
    return max_end


def get_map_by_time(maps, t):
    for k in maps:
        t_range = parse_time_slice(k)
        if t in t_range:
            return maps[k]

    print("No Snapshot Found")


def print_map(m):
    for line in m:
        for ch in line:
            print(ch, end='')
        print("")


def print_map_aligned(m, extra):
    for i in range(len(m)):
        la, lb = m[i], extra[i]
        for ch in la:
            print(ch, end="")
        print("\t\t", end="")
        for ch in lb:
            print(ch, end="")
        print("")


def overlap_map(ma, mb):
    overlaped = []
    for i in range(len(ma)):
        la, lb = ma[i], mb[i]
        newl = []
        for j in range(len(la)):
            cha, chb = la[j], lb[j]
            if cha == '#' or chb == '#':
                newl.append('#')
            else:
                newl.append("*")
        overlaped.append(newl)
    return overlaped


def overlap_map_by_time(mas, mbs, t):
    ma = get_map_by_time(mas, t)
    mb = get_map_by_time(mbs, t)
    return overlap_map(ma, mb)


def cmd():
    global START_Y
    global START_X
    ch = input()
    input_cmds.append(ch)
    if ch == 'w':
        START_Y -= 1
        if START_Y < 0:
            print("Die")
            exit(0)
        return True
    if ch == 's':
        return True
    if ch == 'x':
        START_Y += 1
        return True
    if ch == 'a':
        START_X -= 1
        return True
    if ch == 'd':
        START_X += 1
        return True
    if ch == 't':
        print("Stop Time")
        global STOP_TIME
        STOP_TIME = 3
        return True
    if ch == 'k':
        if (START_Y == TARGET_Y) and (START_X == TARGET_X):
            print("Kill")
            print("Congrats")
            print(input_cmds)
            exit(0)
        return True
    else:
        return False


def target_mv():
    mv_idx = target_step[TIME_CNT % len(target_step)]
    global TARGET_Y
    global TARGET_X
    if STOP_TIME != 0:
        return
    else:
        if mv_idx == 'w':
            TARGET_Y -= 1
        elif mv_idx == 'x':
            TARGET_Y += 1
        elif mv_idx == 'a':
            TARGET_X -= 1
        elif mv_idx == 'd':
            TARGET_X += 1
        else:
            print("Error")
            exit(1)


if __name__ == '__main__':
    guard1 = read_map("./guard1.txt")
    guard2 = read_map("./guard2.txt")
    max_t = get_max_time(guard1)
    tmp1 = None
    while True:
        m = overlap_map_by_time(guard1, guard2, TIME_CNT)
        if m[START_Y][START_X] == '#':
            print("Detected -> You Die")
            exit(0)
        tmp1 = m[START_Y][START_X]
        tmp2 = m[TARGET_Y][TARGET_X]
        m[START_Y][START_X] = '^'
        m[TARGET_Y][TARGET_X] = '@'
        if TIME_CNT + 1 <= max_t:
            next_map = overlap_map_by_time(guard1, guard2, TIME_CNT + 1)
            print_map_aligned(m, next_map)
        else:
            print_map(m)
        m[START_Y][START_X] = tmp1
        m[TARGET_Y][TARGET_X] = tmp2
        cmd()
        target_mv()
        if STOP_TIME != 0:
            STOP_TIME -= 1
            continue
        else:
            TIME_CNT += 1
        print("=========================================")
