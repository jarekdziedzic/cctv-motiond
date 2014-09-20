#!/bin/bash
if [ $# -ne 1 ]; then
    echo "This script takes directory with images as the only argument."
    echo "Got arguments $@"
    exit 1
fi

echo "Analysing $1"
cd $1

find . -maxdepth 1 -daystart -mtime 8 | xargs -r -- bash -c 'dir=$0; echo "deleting ${dir}"; rm -r $dir'  || true
cd -

