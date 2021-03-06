#!/bin/bash

while true
do
    for i in 1 2 3; do 
        dir="/store/cctv/cam$i"
        cd $dir || (echo "Could not cd into $dir. Aborting."; exit 1)
        today=`date +"%Y-%m-%d"`
        for j in $today/* ; do /usr/local/bin/find-motion-in-dir.sh $j; done
        /usr/local/bin/delete-old-images.sh $dir
        cd -
    done
    sleep 15
done

exit 0
