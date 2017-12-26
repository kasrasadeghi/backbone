from subprocess import check_output, STDOUT, CalledProcessError
from numpy import average

def call(cmd):
    try:
        result = check_output(cmd.split(), stderr=STDOUT, timeout=100).decode('latin-1'), 0
    except CalledProcessError as e:
        result = e.output.decode('latin-1'), e.returncode
    return result

if __name__ == '__main__':
    filename = "./while"
    l = []
    print("Testing:", filename)
    for _ in range(0, 10):
        output = call("time " + filename)
        stuff = output[0].split()
        print('value:', stuff[1], end=" ")
        print(stuff[4][2:], end=" ")
        l.append(float(stuff[4][2:7]))
        print(stuff[9], end=" ")
        print()
    print(l)
    print(average(l))
