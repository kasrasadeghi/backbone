all: build run

prep:
	if [ ! -d cmake-build-dir ]; then mkdir cmake-build-dir; fi
	cd cmake-build-dir && \
		cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..

build: ./cmake-build-dir/Makefile
	cd cmake-build-dir && make

run: ./cmake-build-dir/backbone
	cd cmake-build-dir && ./backbone
