from subprocess import check_output, STDOUT, CalledProcessError
from os.path import isfile
from os import chdir, getcwd, listdir as ls
import sys

# region  === util ===


def call(cmd):
    try:
        result = check_output(cmd.split(), stderr=STDOUT, timeout=5).decode('utf-8')
    except CalledProcessError as e:
        result = e.output.decode('utf-8') + 'exit code: ' + str(e.returncode)
    print(result)


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
_output    = ''
_reference = ''


def test_output(test_name, output_ending, reference_ending):
    with cd(_test_dir):
        with open(test_name + output_ending, 'r') as f:
            output = f.read()
        with open(test_name + reference_ending, 'r') as f:
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
    with cd(_test_dir):
        return isfile(_input.replace('%', name))


def valid_tests():
    return [x for x in map(basename, ls('gen-tests')) if valid_test(x)]


def test(file_name):
    print('testing', file_name)



def testall():
    if not valid_tests():
        print(' nothing to do...')
        return
    else:
        for file_name in valid_tests():
            test(file_name)


def main(*argn):

    # TODO do for all elements
    test_suite = argn[0]

    def config(name):
        if line.startswith(name):
            value = line.split(name + ':')[1].strip()
            code = 'global _' + name + '\n_' + name + ' = "' + value + '"'
            print(code)
            exec(code)

    with open(test_suite + '.ktest', 'r') as f:
        for line in f:
            config('test_dir')
            config('input')
            config('run')
            config('output')
            config('reference')

    testall()


if __name__ == '__main__':
    main(*sys.argv[1:])
