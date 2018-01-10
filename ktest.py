from subprocess import check_output, STDOUT, CalledProcessError
from os.path import isfile
from os import chdir, getcwd, listdir as ls
import sys


# region  === util ===

def ok(string):
  return OK_BLUE + string + END_C
def good(string):
  return OK_GREEN + string + END_C
def fail(string):
  return FAIL + string + END_C

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
  def __init__(t, s, name: str):
    t.name = name
    t.s = s

  def output(t):
    return t.s.output.replace('%', t.name)

  def reference(t):
    return t.s.reference.replace('%', t.name)
  
  def diff_result(t, stdout):
    with cd(t.s.test_dir):
      output_name = t.output()
      ref_name    = t.reference()

      if isfile(output_name):
        with open(output_name, 'r') as f:
          output = f.read()
        with open(ref_name, 'r') as f:
          reference = f.read()

        return output.strip(), reference.strip()


      output = stdout[0]
      if stdout[1] != 0:
        output += "exit code: " + str(stdout[1])

      with open(ref_name, 'r') as f:
        reference = f.read()

      return output.strip(), reference.strip()

  def run(t):

    output = 'OUTPUT ERROR'
    reference = 'REFERENCE ERROR'
    try:
      # run command for test
      assert t.s.run != '' or t.s.dir_run != ''

      if t.s.run != '':
        stdout = call(t.s.run.replace('%', t.name))
      else:
        with cd(t.s.test_dir):
          stdout = call(t.s.dir_run.replace('%', t.name))

      output, reference = t.diff_result(stdout)

      if output != reference:
        raise ValueError
    except ValueError as e:
      # execute failure
      print('[ ', fail('FAIL'), ' ]', t.name)
      if t.s.print_fail:
        print('expected:')
        print(reference)
        print('output:')
        print(output)
      return 0

    except BaseException as e:
      # some weird exception
      raise e
    else:
      if t.s.print_pass:
        print('[ ', ok('PASS'), ' ]', t.name)
      return 1
    finally:
      # cleanup
      if t.s.cleanup != '':
        with cd(t.s.test_dir):
          call(t.s.cleanup.replace('%', t.name))


class TestSuite:
  def __init__(s, name):
    s.print_fail = False
    s.print_pass = False
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

  def check_files(t, *files):
    with cd(t.test_dir):
      result = all(isfile(f) for f in files)
    return result

  def valid_test(t, name):
    def p(string):
      return string.replace('%', name)

    if t.require == '':
      return t.check_files(p(t.input), p(t.reference))
    else:
      return t.check_files(*list(map(p, t.require.split())))

  def valid_tests(s):
    return sorted(list({x for x in map(basename, ls(s.test_dir)) if s.valid_test(x)}))

  def test_all(t):
    print("[  TEST  ]", t.name)

    tests = t.valid_tests()
    test_count = len(tests)
    valid_count = 0
    if not tests:
      print(' nothing to do ...')
      exit(0)
    else:
      for file_name in tests:
        valid_count += t.test(file_name)
    result = valid_count, test_count

    t.print_result(result)

  def print_result(s, result):
    valid, count = result
    print("[ RESULT ] ", end="")
    if valid == count:
      print(ok("ALL OK"))
    else:
      print(valid, "/", count, "tests passed")

  def test_each(s, tests):
    for t in tests:
      s.test(t)

  def test(s, test_name):
    test = Test(s, test_name)
    return test.run()


def main():
  for arg in sys.argv:
    if '.' not in arg:
      print()
      test_suite = TestSuite(arg)
      test_suite.test_all()

  #TODO if %.suite then set test_suite?
  #TODO if %.test then test name?
  #TODO if {test}/{name} test name in test_suite?

if __name__ == '__main__':
  main()
