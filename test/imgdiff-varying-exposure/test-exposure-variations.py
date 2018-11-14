#! /usr/bin/env python
__author__ = 'jarek'

from subprocess import check_output

def imgdiff(img):
    output = check_output(["../../imgdiff", img[0], img[1], "/tmp"])
    #sprint "output of %r is '%s'" %(img, output)
    return int(output)


def main():
    images = [ ["images/brighter.jpeg", "images/darker.jpeg"],
               ["images/brighter2.jpeg", "images/darker2.jpeg"],
               ["images/brighter3.jpeg", "images/darker3.jpeg"],
               ["images/brighter4.jpeg", "images/darker4.jpeg"],
               ["images/brighter5.jpeg", "images/darker5.jpeg"]
            ]

    DIFFERENT_PIXEL_LIMIT = 480
    for imgs in images:
        print "Comparing ", imgs
        rv = imgdiff(imgs)
        if rv < DIFFERENT_PIXEL_LIMIT:
            print "OK"
        else:
            print "FAIL %r, imgdiff=%r" % (imgs, rv)
            return 1

        imgs = imgs[::-1]
        print "Comparing ", imgs
        
        rv = imgdiff(imgs)
        if rv < DIFFERENT_PIXEL_LIMIT:
            print "OK"
        else:
            print "FAIL %r, imgdiff=%r, limit=%r" % (imgs, rv, DIFFERENT_PIXEL_LIMIT)
            return 1

    return 0

if __name__ == "__main__":
    main()
