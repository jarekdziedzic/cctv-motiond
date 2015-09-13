#!/bin/bash

function usage()
{
    echo -n "This test script takes as an input the path to the imgdiff tool. "
    echo -n "This way one can run the same set of tests on all of the implementations "
    echo    "of the tool."
}

if [[ ! $# -eq 1 ]]; then
    echo "Invalid parameter count: $#. Aborting."
    usage
    exit 1
fi

imgdiff=$1

if [[ ! -f $imgdiff ]]; then
    echo "imgdiff not found at provided location. Aborting."
    exit 1
fi

scratchdir=`mktemp -d`
echo "Using scratchdir $scratchdir"

#test diff of different images
output=$($imgdiff images/1.jpeg images/2.jpeg $scratchdir)
exitcode=$?

#imgdiff prints 0 when images are similar and non zero value of different pixel count when they are different
#any other status is an error

#try comparing different images first
echo "imgdiff: $output"
if [[ $exitcode -ne "0" ]]; then
    echo "imgdiff returned an error. Aborting."
    exit $exitcode
fi

if [[ $output -eq "0" ]]; then
    echo "images should have been flaged as different"
    exit 1
fi

#ensure that cropped images which show the moving object
#are created along with their thumbnail versions
for i in 1-1.jpeg 1-1s.jpeg 2-2.jpeg 2-2s.jpeg; do
    croppath="$scratchdir/$i"
    if [[ ! -f $croppath ]]; then
        echo "imgdiff should have created $croppath"
        exit 1
    fi
done
