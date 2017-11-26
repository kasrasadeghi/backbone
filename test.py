#!/usr/bin/python
from subprocess import check_output, STDOUT, CalledProcessError
import sys
import os


def call(cmd):
    try:
        result = check_output(cmd.split(), stderr=STDOUT, timeout=5).decode('utf-8')
    except CalledProcessError as e:
        result = str(e.returncode) + e.output.decode('utf-8')
    return result


def testall():
    test_names = [x[:-3] for x in os.listdir('parser-tests')
                  if x.endswith('.bb')
                  if os.path.isfile(x[:-3] + 'ok')]
    if not test_names:
        print(' nothing to do...')
        return
    for filename in test_names:
        test(filename)


def test(testname):
    print(" --- ", testname, end=" ...")
    output = call('cmake-build-debug/backbone ' + testname + ".bb")
    with open(testname + ".ok", "r") as f:
        reference = f.read()
    output = output.strip()
    if output != reference:
        print(" fail --- ")
        print(" EXPECTED:")
        print(reference)
        print(" FOUND:")
        print(output)
    else:
        print(" pass --- ")


if len(sys.argv) == 1:
    testall()
else:
    test(sys.argv[1])
