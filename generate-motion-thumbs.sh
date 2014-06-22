if [ $# -ne 1 ]
        then echo "This script takes directory with images as the only argument."
        exit 1
fi

cd $1
#if there was some motion detected, generate thumbnails, unless they already exist
#this is checked on file by file basis
if [ -d "$1/0_ciekawe" ]; then
	if [ ! -d "$1/0_ciekawe/thumbs" ]; then
		mkdir "$1/0_ciekawe/thumbs"
	fi

	if [ ! -d "$1/0_ciekawe/small" ]; then
		mkdir "$1/0_ciekawe/small"
	fi
	
	JPEGS_WILDCARD="$1/0_ciekawe/*.jpeg"
 
	for i in $JPEG_WILDCARD
	do
		if [ $i -ne $JPEG_WILDCARD ]; then

			base=`basename $i`
			thumb="0_ciekawe/thumbs/$base"
			small="0_ciekawe/small/$base"
			#echo "base=$base \n"
			#echo "thumb=$thumb\n"
			#echo "i=$i\n"	
			if [ ! -f $thumb ]; then
				convert -resize "60x60" -quality 50 $i $thumb
			fi

			if [ ! -f $small ]; then
				convert -resize "800x600" $i $small
			fi
		fi
	done
fi

cd -

