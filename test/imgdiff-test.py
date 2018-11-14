#! /usr/bin/env python
__author__ = 'jarek'

from subprocess import check_output

def imgdiff(img):
    output = check_output(["../imgdiff", img[0], img[1], "/tmp"])
    #sprint "output of %r is '%s'" %(img, output)
    
    return int(output)

def main():
    images_nodiff = [ ["images/5.jpeg", "images/6.jpeg"],
                      ["images/8.jpeg", "images/9.jpeg"],
                    ]
    images_diff = [ ["images/6.jpeg", "images/7.jpeg"],
                    ]
    for imgs in images_nodiff:
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


    for imgs in images_diff:
        print "Comparing ", imgs
        rv = imgdiff(imgs)
        if rv == 0:
            print "FAIL %r, imgdiff=%r" % (imgs, rv)
            return 1
        else:
            print "OK"

        imgs = imgs[::-1]
        print "Comparing ", imgs
        rv = imgdiff(imgs)
        if rv == 0:
            print "FAIL %r, imgdiff=%r" % (imgs, rv)
            return 1
        else:
            print "OK"
    return 0

if __name__ == "__main__":
    main()
