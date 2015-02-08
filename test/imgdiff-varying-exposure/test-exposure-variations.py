#! /usr/bin/env python
__author__ = 'jarek'

from subprocess import call


def main():
    images = [ ["images/brighter.jpeg", "images/darker.jpeg"],
               ["images/brighter2.jpeg", "images/darker2.jpeg"],
               #["images/brighter3.jpeg", "images/darker3.jpeg"],
               ["images/darker3.jpeg", "images/brighter3.jpeg"]]

    for img in images:
        print "Comparing ", img
        rv = call(["../../imgdiff", img[0], img[1], "/tmp"])

        if rv == 0:
            print "OK"
        else:
            print "FAIL ", img
            return 1

    return 0

if __name__ == "__main__":
    main()
