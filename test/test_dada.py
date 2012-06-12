#!/usr/bin/env python

import subprocess
import unittest


class DadaTest(unittest.TestCase):

    def test_defaults(self):
        stdoutdata, _  = dada()
        self.assertIn('\t', stdoutdata)
        self.assertGreaterEqual(len(stdoutdata), 1024)
        self.assertLessEqual(len(stdoutdata), 8 * 1024)

    def test_field_separator(self):
        stdoutdata, _ = dada('-f,')
        self.assertIn(',', stdoutdata)

    def test_size(self):
        stdoutdata, _ = dada('-s1m')
        self.assertGreaterEqual(len(stdoutdata), 1024 * 1024)

    def test_version(self):
        stdoutdata, _ = dada('-v')
        self.assertEqual(stdoutdata, '0.0.1\n')

    def test_usage(self):
        _, stderrdata = dada('-h')
        self.assertEqual(stderrdata,
            'Usage: dada [-f field-separator] [-s size] [-v]\n')


def dada(*opts):
    args = ['../dada']
    args.extend(opts)
    process = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return process.communicate()


if __name__ == "__main__":
    unittest.main()
