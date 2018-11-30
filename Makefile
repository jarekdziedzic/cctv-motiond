dest=/usr/local/bin
#CXX_FLAGS=-g -O0
CXX_FLAGS=-O3
all: check

imgdiff: imgdiff-src/*.cpp Makefile
		g++ -std=c++17 imgdiff-src/*.cpp $(CXX_FLAGS) -Wl,--as-needed  -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -pthread -lpthread -lstdc++fs -O3 -g -o imgdiff

imgdiff-prof: imgdiff
		g++ -std=c++17 imgdiff-src/*.cpp $(CXX_FLAGS) -Wl,--as-needed -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -g -pg -O3 -pthread -lpthread -lstdc++fs -o imgdiff-prof

run-oprofile: imgdiff-prof
		sudo operf ./imgdiff-prof test/images/big1.jpeg test/images/big2.jpeg /tmp && opreport -c 
		#&& sudo rm -rf oprofile_data

run-perf: imgdiff
		perf record -g -F1000 ./imgdiff test/images/big1.jpeg test/images/big2.jpeg /tmp; perf annotate

run-gprof: imgdiff-prof
		./imgdiff-prof test/images/big1.jpeg test/images/big2.jpeg /tmp; gprof --flat-profile ./imgdiff-prof gmon.out

run-callgrind: imgdiff
		valgrind --separate-threads=yes --tool=callgrind  ./imgdiff test/images/big1.jpeg test/images/big2.jpeg /tmp

run-timed:
		time ./imgdiff test/images/big1.jpeg test/images/big2.jpeg /tmp
# -v --dump-every-bb=10000000 
clean:
		rm imgdiff imgdiff-prof

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
