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

    path = 'skel.spec'
    with open(path, "r") as f:
        lines = f.readlines()

    output = []
    for l in lines:
        if('Version:' in l):
            l = 'Version: ' + ver_str + '\n'
        if('Release:' in l):
            l = 'Release: ' + args[1] + '%{?dist}\n'
        output.append(l)

    path = 'rpmbuild/SPECS/cello.spec'
    with open(path, "w") as f:
        f.writelines(output)

    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))
