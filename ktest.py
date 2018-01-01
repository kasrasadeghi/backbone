from subprocess import check_output, STDOUT, CalledProcessError
from os.path import isfile
from os import chdir, getcwd, listdir as ls
import sys

# region  === util ===


def call(cmd):
    try:
        result = check_output(cmd.split(), stderr=STDOUT, timeout=5).decode('latin-1'), 0
    except CalledProcessError as e:
        result = e.output.decode('latin-1'), e.returncode
    return result


class cd:
    def __init__(self, dir_name):
        self.dir_name = dir_name
        self.prev_dir = getcwd()

    def __enter__(self):
        chdir(self.dir_name)

    def __exit__(self, *l):
        chdir(self.prev_dir)


def basename(path):
    return path.split("/")[-1].split(".")[0]

# endregion


_test_dir  = ''
_input     = ''
_run       = ''
_dir_run   = ''
_output    = ''
_reference = ''
_cleanup   = ''
_require   = ''

OK_GREEN = '\033[92m'
OK_BLUE = '\033[94m'
FAIL = '\033[91m'
END_C = '\033[0m'


def test_output(test_name, output, reference):
    def wrap(s, color):
        return color + s + END_C

    print('[ ', end='')
    if output == reference:
        print(wrap('PASS', OK_BLUE), ']', test_name)
    else:
        print(wrap('FAIL', FAIL), ']', wrap(test_name, FAIL))


def check_files(*files):
    with cd(_test_dir):
        result = all(isfile(f) for f in files)
    return result


def valid_test(name):
    def p(s):
        return s.replace('%', name)
    if _require == '':
        return check_files(p(_input), p(_reference))
    else:
        return check_files(*list(map(p, _require.strip().split())))


def valid_tests():
    return sorted(list({x for x in map(basename, ls(_test_dir)) if valid_test(x)}))


def test_run(test_name):
    assert _run != '' or _dir_run != ''

    if _run != '':
        stdout = call(_run.replace('%', test_name))
    else:
        with cd(_test_dir):
            stdout = call(_dir_run.replace('%', test_name))
    return stdout


def test_cleanup(test_name):
    if _cleanup != '':
        with cd(_test_dir):
            call(_cleanup.replace('%', test_name))


def check_result(test_name, stdout):
    with cd(_test_dir):
        output_name = _output.replace('%', test_name)
        ref_name    = _reference.replace('%', test_name)
        if not isfile(output_name):
            assert _output == ''
            output = stdout[0]
            if stdout[1] != 0:
                output += "exit code: " + str(stdout[1])
        else:
            with open(output_name, 'r') as f:
                output = f.read()
        with open(ref_name, 'r') as f:
            reference = f.read()
    return output.strip(), reference.strip()


def test(test_name):
    stdout = test_run(test_name)
    output, reference = check_result(test_name, stdout)
    test_output(test_name, output, reference)
    test_cleanup(test_name)
    return 1 if output == reference else 0


def testall():
    tests = valid_tests()
    test_count = len(tests)
    valid_count = 0
    if not tests:
        print(' nothing to do...')
        return
    else:
        for file_name in tests:
            valid_count += test(file_name)
    return valid_count, test_count


def print_test_suite_result(test_suite_result):
    valid, count = test_suite_result
    print("[      ] ", end="")
    if valid == count:
        print("ALL OK")
    else:
        print(valid, "/", count, "tests passed")


def main(*args):
    global _test_dir
    global _input
    global _run
    global _dir_run
    global _output
    global _reference
    global _cleanup
    global _require
    _test_dir  = ''
    _input     = ''
    _run       = ''
    _dir_run   = ''
    _output    = ''
    _reference = ''
    _cleanup   = ''
    _require   = ''
    test_suite = args[0]

    def config(name):
        if line.startswith(name):
            value = line.split(name + ':')[1].strip()
            code = 'global _' + name + '\n_' + name + ' = "' + value + '"'
            exec(code)

    with open(test_suite + '.ktest', 'r') as f:
        for line in f:
            config('test_dir')
            config('input')
            config('run')
            config('dir_run')
            config('output')
            config('reference')
            config('cleanup')
            config('require')

    if len(args) == 1:
        print("[ TEST ]", test_suite)
        test_suite_result = testall()
        print_test_suite_result(test_suite_result)
    else:
        for t in args[1:]:
            test(t)


def pre_main():
    if len(sys.argv) != 1:
        main(*sys.argv[1:])
    else:
        for test_file_name in [x[:-6] for x in ls('.') if x.endswith('.ktest')]:
            print()
            main(test_file_name)


if __name__ == '__main__':
    pre_main()