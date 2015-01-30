#! /usr/bin/env python
__author__ = 'jarek'

from subprocess import call

def main():
    rv = call(["../../imgdiff", "images/brighter.jpeg", "images/darker.jpeg", "/tmp"])

    if rv == 0:
        print "OK"
        return 0
    else:
        print "FAIL"
        return 1

if __name__ == "__main__":
    main()
