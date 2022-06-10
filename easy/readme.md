## very_secure_crackme
- flag{We1c0m3_to_pl4y_Bro!}

1. 本身应该可以很快定位到调用python的地方，根据offset=0x4000, 用dd切出隐藏的py文件`dd if=crackme of=./temp.py skip=16384 bs=1`

2. 代码如下，分析可以发现里面有一个加密后的密文硬编码到文件中，`not_decode`函数可以直接拿来解密得到flag，rot系列的加解密是相同的操作

```python
very_secure_secret = "7=28L(6`4_>b0E@0A=cJ0qC@PN"
alphabet = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ"+ \
            "[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

def not_decode(secret):
    rotate_const = 47
    decoded = ""

    for c in secret:
        index = alphabet.find(c)
        original_index = (index + rotate_const) % len(alphabet)
        decoded = decoded + alphabet[original_index]

    print(decoded)

def choose_greatest():
    print("AI-based IQ Test")
    user_value_1 = input("What's your lucky number ")
    user_value_2 = input("What's your birthday ")
    greatest_value = user_value_1

    if user_value_1 > user_value_2:
        greatest_value = user_value_1
    elif user_value_1 < user_value_2:
        greatest_value = user_value_2

    print("You are a talent!")

if __name__ == "__main__":
    choose_greatest()
```

### build
```
.
├── build.sh
├── crackme.c 	生成python caller
├── crackme.py	实际的python脚本
└── target_append.py 将binary扩充到指定字节数
```
题目生成
> ./build.sh


## babygame
- flag{new_ye@r}

0. 总共有两个文件，unk_box显示我们需要找巫师了解其用法
1. 运行游戏会发现这是一个无法完成的游戏，拖到IDA中反编译可以发现一些逻辑:
	- 击败恶龙后会获得 20220131 这样一串字符
	- 游戏若是以gdb打开（ptrace(TRACEME)检测进程是否被调试）则会触发找到巫师的剧情
	- 巫师会将unk_box重命名为game_helper

2. game_helper 运行后会根据输入改写game中的flag, 此时使用`20220131`可获得真正的flag

- PS. 题目中game_helper和game之间用了共享内存，使得game_helper能够改写game中的flag。希望能通过运行和调试找到flag,而不是纯粹的静态分析.

### build
```
.
├── bag
│   └── unk_box.c  解码逻辑放在里面
├── build.sh	   
├── game.c	   游戏主逻辑
└── .info	   共享内存需要
    └── key

```

> ./build.sh


## wind0ws
- flag{We11_d0ne_binary_h4cker}

1. 题目中总共有两个文件，flag_inside并不是一个真正的exe,只包含了DOS头，其余部分是我自己定义的格式。用题目中给的loader调用之可以打印binary的基本信息以及hello world和banner

2. 本题保留了原始的函数名，应该能为解题带来方便。程序一直在读取flag_inside, 执行读取的序列如下:
	- locate_start
		- 0x3C 读取一个int作为跳转地址 addr
		- 跳转到 addr

	- read_basic_info
		- addr 共计读取0x34个字节
		- addr + 0x00 -- name
		- addr + 0x10 -- info
		- addr + 0x20 -- build info
		- addr + 0x30 -- entry_count (4 Bytes)
	
	- read_entry 调用 entry count 次, 每次读取0xC个字节
		- entry + 0x0 -- 代表资源是code/chart
		- entry + 0x1 -- 是否启用资源
		- entry + 0x4 -- offset
		- entry + 0x8 -- entry size
	
3. 用二进制编辑器打开flag_inside, 定位到0x130处即是entry count, 总共有4个entry, 然而运行时似乎只有helloworld和banner信息，还有两个资源段未被启用.

4. 可以将资源段全改为启用状态，或者仅修改隐藏的code段的启用状态为1即可(0x141处字节改为1)

- PS. 避免逆向run_as_code_section
- PSS. 文件格式如下
```
0x0		+-------------------------+
		|	   DOS	  	  |
0x3C	--------|	  Header	  |
	|	|			  |
Addr	------->+-------------------------+
		|	 Binary Header    |
Addr+0x34	+-------------------------+
		|     entry 0 (12B)	  |
		+-------------------------+
		|     entry 1 (12B)       |
		+-------------------------+
		.	...........       .
		.    (MAX 16 entryies)    .
		+-------------------------+
		.			  .
		. 	...... (blank)    .
0x1000		+-------------------------+
		|     Section  0	  |
0x2000		+-------------------------+
		|     Section  1 	  |
0x3000		+-------------------------+
		|	...........	  |
......
```


### build

```
.
├── dump.c	 生成伪DOS二进制文件
├── dump_code.py 负责生成code段的OPCode和参数字节码
├── flag_inside  修改后，已启用所有段的binary
├── loader.c 	 加载器，也有运行功能
└── resrc	 生成时需要用到的各种资源
    ├── example.code
    ├── flag.code
    ├── jiaran.txt
    ├── pure_dos_hdr
    └── windows.txt
```

> ./build.sh
