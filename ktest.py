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


OK_GREEN = '\033[92m'
OK_BLUE = '\033[94m'
FAIL = '\033[91m'
END_C = '\033[0m'


class Test:
  def __init__(s, t, name: str):
    s.name = name
    s.t = t
  
  def diff_result(s, stdout):
    with cd(s.t.test_dir):
      output_name = s.t.output.replace('%', s.name)
      ref_name    = s.t.reference.replace('%', s.name)

      if not isfile(output_name):
        assert s.t.output == ''
        output = stdout[0]
        if stdout[1] != 0:
          output += "exit code: " + str(stdout[1])

      else:
        with open(output_name, 'r') as f:
          output = f.read()
      with open(ref_name, 'r') as f:
        reference = f.read()
      return output.strip(), reference.strip()
    
  def run(s):
    def ok(string):
      return OK_BLUE + string + END_C
    def fail(string):
      return FAIL + string + END_C

    output = 'OUTPUT ERROR'
    reference = 'REFERENCE ERROR'
    try:
      # run command for test
      assert s.t.run != '' or s.t.dir_run != ''

      if s.t.run != '':
        stdout = call(s.t.run.replace('%', s.name))
      else:
        with cd(s.t.test_dir):
          stdout = call(s.t.dir_run.replace('%', s.name))

      output, reference = s.diff_result(stdout)

      if output != reference:
        raise ValueError
    except ValueError as e:
      # execute failure
      print('[ ', fail('FAIL') , ' ]', s.name)
      print('expected:')
      print(reference)
      print('output:')
      print(output)
      return 0

    except BaseException as e:
      # some weird exception
      raise e
    else:
      # print('[ ', ok('PASS'), ' ]', s.name)
      # everything is fine
      return 1
    finally:
      # cleanup
      if s.t.cleanup != '':
        with cd(s.t.test_dir):
          call(s.t.cleanup.replace('%', s.name))


class TestSuite:
  def __init__(s, name):
    s.name = name

    with open(name + '.ktest', 'r') as f:
      options = \
        ['test_dir',  'input',   'run', 'dir_run', 'output',
         'reference', 'cleanup', 'require']
      d = {option:'' for option in options}
      for line in f:
        option = line.split(':')[0]
        value = ':'.join(line.split(':')[1:]).strip()
        d[option] = value

      for option, value in d.items():
        setattr(s, option, value)

  def check_files(s, *files):
    with cd(s.test_dir):
      result = all(isfile(f) for f in files)
    return result

  def valid_test(s, name):
    def p(string):
      return string.replace('%', name)

    if s.require == '':
      return s.check_files(p(s.input), p(s.reference))
    else:
      return s.check_files(*list(map(p, s.require.split())))

  def valid_tests(s):
    return sorted(list({x for x in map(basename, ls(s.test_dir)) if s.valid_test(x)}))

  def test_all(s):
    print("[  TEST  ]", s.name)

    tests = s.valid_tests()
    test_count = len(tests)
    valid_count = 0
    if not tests:
      print(' nothing to do ...')
      exit(0)
    else:
      for file_name in tests:
        valid_count += s.test(file_name)
    result = valid_count, test_count

    s.print_result(result)

  def print_result(s, result):
    valid, count = result
    print("[ RESULT ] ", end="")
    if valid == count:
      print("ALL OK")
    else:
      print(valid, "/", count, "tests passed")

  def test_each(s, tests):
    for t in tests:
      s.test(t)

  def test(s, test_name):
    test = Test(s, test_name)
    return test.run()


def main(*args):
  test_suite = TestSuite(args[0])
  if len(args) == 1:
    test_suite.test_all()
  else:
    test_suite.test_each(args[1:])


def pre_main():
  if len(sys.argv) != 1:
    main(*sys.argv[1:])
  else:
    for suite_name in [x[:-6] for x in ls('.') if x.endswith('.ktest')]:
      print()
      main(suite_name)


if __name__ == '__main__':
  pre_main()
