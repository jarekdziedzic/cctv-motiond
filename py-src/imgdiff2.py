#!/usr/bin/python

from sys import argv, exit
import cv, cv2
import numpy as np
import os

script, imgpath1, imgpath2, cropdir = argv

img1 = cv2.imread(imgpath1)
img2 = cv2.imread(imgpath2)

img1grey = cv2.cvtColor(img1, cv.CV_RGB2GRAY)
img2grey = cv2.cvtColor(img2, cv.CV_RGB2GRAY)
imgdiff = cv2.absdiff(img1grey, img2grey)
stuff,imgdiff = cv2.threshold(imgdiff, 30, 255, cv.CV_THRESH_BINARY)
kernel = cv2.getStructuringElement(cv2.MORPH_CROSS,(3,3))
imgdiff = cv2.erode(imgdiff, kernel)

#dilate to bring close contours together
kernel2 = cv2.getStructuringElement(cv2.MORPH_CROSS,(40,40))
#imgdiff = cv2.dilate(imgdiff, kernel2)

different = cv2.countNonZero(imgdiff) > 20

if different:
    contours,hierarchy = cv2.findContours(np.asarray(imgdiff[:,:]), cv.CV_RETR_EXTERNAL, cv.CV_CHAIN_APPROX_SIMPLE)
    img2numpy = np.asarray(img2[:,:]);
    areas = []
    for c in contours:
        areas.append(cv2.contourArea(c))
    biggestIdx = areas.index(sorted(areas, reverse = True)[0])
    x,y,w,h = cv2.boundingRect(contours[biggestIdx])
#    print "\nContour:", x,y,w,h
    crop1 = img1[y:y+h,x:x+w]
    crop2 = img2[y:y+h,x:x+w]

    if not os.path.exists(cropdir):
        os.makedirs(cropdir)  

    cv2.imwrite(cropdir + "/" + os.path.splitext(os.path.basename(imgpath1))[0] + "-1.jpeg", crop1)
    cv2.imwrite(cropdir + "/" + os.path.splitext(os.path.basename(imgpath2))[0] + "-2.jpeg", crop2)

exit(different);

