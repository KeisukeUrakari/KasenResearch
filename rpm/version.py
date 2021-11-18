#!/usr/bin/env python3

import sys

def main(args):
    path = './src/version.h'
    with open(path) as f:
        lines = f.readlines()

    ver_str = ''    
    for l in lines:
        if ('VER_STR' in l):
            ver_str = l.split()[2].replace('"', '')
            break

    if len(ver_str) < 1:
        return 1

    print(ver_str)

    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))
