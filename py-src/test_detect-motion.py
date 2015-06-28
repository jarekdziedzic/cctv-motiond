#! /bin/env python
from detect_motion import *
import tempfile
import shutil
from nose.tools import *

###Tests for get_images

"""Creates dirs and files in scratch space directory. Returns the directory and files"""
@nottest
def create_test_data(files, dirs=[]):
    #create scratch directory
    scratch = tempfile.mkdtemp()
    for d in dirs:
        os.mkdir(scratch + '/' + d)
    outfiles = []
    for f in files:
        outfiles.append(scratch + '/' + f)
        open(outfiles[-1], 'w').close()
    return scratch, outfiles


"""Simple test for finding images in directory"""
def test_get_two_images_same_dir():
    scratch, files = create_test_data(files=['a.jpeg', 'b.jpeg'])
    try:
        filelist = get_images(scratch)
        eq_(sorted(filelist), sorted(files))
    finally:
        shutil.rmtree(scratch)


"""Tests that images in subdirectories are found too"""
def test_get_two_images_subdirs():
    scratch, files = create_test_data(files=['a/a.jpeg', 'b.jpeg'], dirs=['a'])
    try:
        filelist = get_images(scratch)
        eq_(filelist, files)
    finally:
        shutil.rmtree(scratch)
