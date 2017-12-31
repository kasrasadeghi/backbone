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


def test_output(test_name, stdout):
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
    print(' --- ', test_name, end=' ...')
    reference = reference.strip()
    output = output.strip()
    if output != reference:
        print(' fail --- ')
        print(' EXPECTED:')
        print(reference)
        print(' FOUND:')
        print(output)
    else:
        print(' pass --- ')


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


def test(test_name):
    assert _run != '' or _dir_run != ''

    if _run != '':
        stdout = call(_run.replace('%', test_name))
    else:
        with cd(_test_dir):
            stdout = call(_dir_run.replace('%', test_name))
    test_output(test_name, stdout)
    if _cleanup != '':
        with cd(_test_dir):
            call(_cleanup.replace('%', test_name))


def testall():
    tests = valid_tests()
    if not tests:
        print(' nothing to do...')
        return
    else:
        for file_name in tests:
            test(file_name)


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
        testall()
    else:
        for t in args[1:]:
            test(t)


if __name__ == '__main__':
    if len(sys.argv) != 1:
        main(*sys.argv[1:])
    else:
        for test_file_name in [x[:-6] for x in ls('.') if x.endswith('.ktest')]:
            print("testing", test_file_name)
            main(test_file_name)