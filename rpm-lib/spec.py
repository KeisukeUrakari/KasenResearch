#!/usr/bin/env python3

import sys

def main(args):
    path = 'skel.spec'
    with open(path, "r") as f:
        lines = f.readlines()

    output = []
    for l in lines:
        if('Version:' in l):
            l = 'Version: ' + args[2] + '\n'
        if('Release:' in l):
            l = 'Release: ' + args[3] + '%{?dist}\n'
        output.append(l)

    path = 'rpmbuild/SPECS/' + args[1] + '.spec'
    with open(path, "w") as f:
        f.writelines(output)

    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))
