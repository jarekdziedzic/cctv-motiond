dest=/usr/local/bin

all: check

imgdiff: imgdiff-src/*.cpp
		g++ -g -std=c++14 imgdiff-src/*.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -O3 -pthread -lpthread -o imgdiff

profile:
		g++ -std=c++14 imgdiff-src/*.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -pg -Os -pthread -lpthread -o imgdiff
clean:
		rm imgdiff

install:
		cp cctv-motiond $(dest)
		cp cctv-motion-detection.sh $(dest)
		cp find-motion-in-dir.sh $(dest)
		cp delete-old-images.sh $(dest)
		cp generate-motion-thumbs.sh $(dest)
		cp highlight-if-different.sh $(dest)
		cp imgdiff.py $(dest)
		cp imgdiff2.py $(dest)
		cp imgdiff $(dest)
		cp kill-cctv-motiond.sh $(dest)


uninstall:
		rm $(dest)/cctv-motiond
		rm $(dest)/cctv-motion-detection.sh
		rm $(dest)/find-motion-in-dir.sh
		rm $(dest)/delete-old-images.sh
		rm $(dest)/generate-motion-thumbs.sh
		rm $(dest)/highlight-if-different.sh
		rm $(dest)/imgdiff.py
		rm $(dest)/imgdiff2.py
		rm $(dest) imgdiff
		rm $(dest) kill-cctv-motiond.sh

check: imgdiff
	cd test; ./imgdiff-test.sh ../imgdiff
	cd test; ./imgdiff-test.py
	cd test/imgdiff-varying-exposure; ./test-exposure-variations.py

perfcheck: imgdiff
	time ./imgdiff test/images/1.jpeg test/images/2.jpeg /tmp 2>/dev/null
