#! /usr/bin/env python
__author__ = 'jarek'

from subprocess import check_output


def imgdiff(img):
    return int(check_output(["../../imgdiff", img[0], img[1], "/tmp"]))

def main():
    images = [ ["images/brighter.jpeg", "images/darker.jpeg"],
               ["images/brighter2.jpeg", "images/darker2.jpeg"],
               ["images/brighter3.jpeg", "images/darker3.jpeg"],
               #["images/brighter4.jpeg", "images/darker4.jpeg"]
               ["images/brighter5.jpeg", "images/darker5.jpeg"],
            ]

    for imgs in images:
        print "Comparing ", imgs
        rv = imgdiff(imgs)
        if rv == 0:
            print "OK"
        else:
            print "FAIL %r, imgdiff=%r" % (imgs, rv)
            return 1

        imgs = imgs[::-1]
        print "Comparing ", imgs
        rv = imgdiff(imgs)
        if rv == 0:
            print "OK"
        else:
            print "FAIL %r, imgdiff=%r" % (imgs, rv)
            return 1

    return 0

if __name__ == "__main__":
    main()
