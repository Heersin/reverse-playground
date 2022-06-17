# 2022.06
## misc-git_warmup
- 题目描述：小丁在为公司开发代码，但第一次用git的他似乎暴露了什么
- 预估难度：低
- flag ： flag{g1t_Leeeeak_rm_fi13_doNt_4get_stash}

### 考察点
1. 了解git的一些用法和特性

### 解题思路
1. 检查下git log,可以发现有个commit是删除了敏感信息，那么check到具体的commit上可以看到泄漏的口令
2. 检查git stash 的内容(git stash list)，git stash apply可在verify.c文件中发现后半部分flag

## re-movmovmov
- 题目描述：输入flag, 就可以获得flag, flag格式为"flag{...}"
- 预估难度：中
- flag：flag{movmovmeow}

### 考察点
出题参考了SusCTF 2018 MoVfuscationl (https://github.com/susers/Writeups/tree/master/2018/SUSCTF/Reverse/movfuscation1)

1. 识别movfuscator
2. 通过字符串及程序行为推测程序的部分信息
3. 暴力破解的脚本

原始程序如下，ubuntu16.04上用[movfuscator](https://github.com/xoreaxeaxeax/movfuscator)编译

```C
#include <stdlib.h>
#include <stdio.h>

const char *flag_nearby = "xored flag nearby";
unsigned char data[24] = {
        0x4c, 0x46, 0x4b, 0x4d, 0x51, 0x47, 0x45, 0x5c, 0x47, 0x45, 0x5c, 0x47, 0x4f, 0x45, 0x5d, 0x57
};
const char *gift = "gift :";

int main(){
        unsigned char input[24];
        scanf("%16s", input);
        int i;
        for (i = 0; i < 16; ++i)
        {
                if (data[i] != (input[i] ^ 42)) {
                        putchar('\n');
                        return 1;
                }
                else
                        putchar(input[i]);
        }
        putchar('\n');
        printf("Congrats FLAG : %s\n", input);
}
```

### 解题思路
#### 解法1
尝试后可以发现，每个正确的字符都会回显，而根据程序字符串信息flag长度为16, 可以写脚本暴力破解各个字节。
```python
import string
import subprocess
import sys
def main():
    bin="./movmovmov"
    flag=""
    last_output = 0
    length=16
    for l in range(length):
        for i in string.printable:
            p = subprocess.Popen("%s" % bin,stdin=subprocess.PIPE,stdout=subprocess.PIPE)
            passwd = flag + i*(length - len(flag)) + "\n"
            p.stdin.write(passwd)
            output = p.stdout.readlines()[0][:-1]
            if len(output) > last_output:
                last_output = len(output)
                flag+=i
                print flag

if __name__=="__main__":
    main()
```
效果
```
❯ python2 crack1.py
f
fl
fla
flag
flag{
flag{m
flag{mo
flag{mov
flag{movm
flag{movmo
flag{movmov
flag{movmovm
flag{movmovme
flag{movmovmeo
flag{movmovmeow
flag{movmovmeow}
```

#### 解法2
字符串未混淆，可以搜索下，能发现"xored flag nearby"的提示，在数据区域可以找到一串有一定规律的神秘数据：
```C
/// LFKMQGE\\GE\\GOE]W
const uint8_t data[16] = 
{
	0x4c, 0x46, 0x4b, 0x4d, 0x51, 0x47, 0x45, 0x5c, 0x47, 0x45, 0x5c, 0x47, 0x4f, 0x45, 0x5d, 0x57
};
```
可以编写脚本用0x00-0xff的key去尝试异或暴破

#### 可能的解法3
movfuscator本身的反混淆工作进展不多(https://github.com/kirschju/demovfuscator)，可能可以


