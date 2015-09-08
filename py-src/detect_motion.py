#! /bin/env python

import os
import sys
from subprocess import check_output,CalledProcessError

"""Returns a list of files in the directory that are images"""
def get_images(dir, exts=['.jpeg']):
    images = []
    for root, dirs, files in os.walk(dir):
        for f in files:
            for e in exts:
                if str(f).endswith(e):
                    images.append('%s/%s' % (root, f))
                    break
    #FIXME: This assumes that alphabetical sort is equivalent to
    #FIXME: chronological sort of images.
    return sorted(images)


def imgdiff(p):
    try:
        return int(check_output(['./imgdiff', p[0], p[1], '/tmp']))
    except CalledProcessError as e:
        #FIXME
        print "boom! %r: %r" % (e.cmd, e.output)
        return 0
    return 0


"""For list of directories get images, organize them into pairs and give them scores
   Return a tuple of two images and their image diff score."""
def find_motion(*dirs):
    scores = []
    for dir in dirs:
        imgs = get_images(dir, ['.jpeg'])
        if len(imgs) > 0:
            pairs = zip(imgs, imgs[1:])
            #print "pairs = %r" % pairs
            scores += [(p, imgdiff(p)) for p in pairs]
        return scores

if __name__ == '__main__':
    print  "Scores = %r" % find_motion(sys.argv[1])