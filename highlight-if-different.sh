#!/bin/bash

echo "imgdiff $1 $2"

cropdir="0_ciekawe/wycinki"
#imgdiff should do it
mkdir -p "0_ciekawe/wycinki" &>/dev/null || true

/usr/local/bin/imgdiff $1 $2 ${cropdir}

if [ $? -eq 66 ]; then
        echo "Intruder found."
	echo "Highlighting ${1} and ${2} in $PWD"
        ln -s `readlink -f ${1}` "0_ciekawe/$(basename ${1})"
        ln -s `readlink -f ${2}` "0_ciekawe/$(basename ${2})"
fi

