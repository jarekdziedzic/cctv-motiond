#!/bin/bash
if [ $# -ne 1 ]; then
    echo "This script takes directory with images as the only argument."
    echo "Got arguments $@"
    exit 1
fi

function diff_and_update_progress()
{
    /usr/local/bin/highlight-if-different.sh $1 $2 | tee -a ".log"
    if [ ${PIPESTATUS[0]} ];
    then
        touch -r $1 ".done"
    fi
}

if [ $1 -nt "$1/.done" ]; then
    echo "Analysing $1"
    cd $1
    export -f diff_and_update_progress
    if [ ! -e  .done ]; then 
        touch -t 197001010000 .done 
    fi
    find *.jpeg -newer .done | grep -v 0_ciekawe | xargs -r -n2 bash -c 'diff_and_update_progress "$@"' _ || true
#    /usr/local/bin/generate-motion-thumbs.sh .
    cd -
fi

