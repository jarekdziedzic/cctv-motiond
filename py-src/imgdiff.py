#!/usr/bin/python

from sys import argv, exit
import cv

if len(argv) == 2:
	exit (0)

script, imgpath1, imgpath2 = argv

img1 = cv.LoadImage(imgpath1)
img2 = cv.LoadImage(imgpath2)

img1grey = cv.CreateImage(cv.GetSize(img1), 8, 1)
img2grey = cv.CreateImage(cv.GetSize(img2), 8, 1)

imgdiff = cv.CreateImage(cv.GetSize(img1), 8, 1)
cv.CvtColor(img1, img1grey, cv.CV_RGB2GRAY)
cv.CvtColor(img2, img2grey, cv.CV_RGB2GRAY)
cv.AbsDiff(img1grey, img2grey, imgdiff)
cv.Threshold(imgdiff, imgdiff, 80, 255, cv.CV_THRESH_BINARY)
cv.Erode(imgdiff, imgdiff)
howDifferent = cv.CountNonZero(imgdiff)
#print "Difference factor %r" % howDifferent
different = howDifferent > 10
exit(different);


