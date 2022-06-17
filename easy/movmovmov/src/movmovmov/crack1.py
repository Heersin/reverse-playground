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
