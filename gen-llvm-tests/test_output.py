#!/usr/bin/python3
from subprocess import check_output, STDOUT, CalledProcessError
from subprocess import call as _call
import sys
import os


def call(cmd):
    try:
        result = check_output(cmd.split(), stderr=STDOUT, timeout=5).decode('utf-8')
    except CalledProcessError as e:
        result = e.output.decode('utf-8') + "exit code: " + str(e.returncode)
    return result


def testall():
    test_names = [x[:-3] for x in os.listdir('.')
                  if x.endswith('.bb')
                  if os.path.isfile(x[:-3] + '.ok.ll')]
    if not test_names:
        print(' nothing to do...')
        return
    for filename in test_names:
        test(filename)


def test(testname):
    # generate output
    _call(('../cmake-build-debug/backbone generateLLVM ' + testname + '.bb').split())
    output = call('make -s from:' + testname).strip()

    # generate reference
    reference = call('make -s from:' + testname + '.ok').strip()

    print(" --- ", testname, end=" ...")
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
