all: prep build run

.PHONY: prep
prep:
	if [ ! -d cmake-build-debug ]; then mkdir cmake-build-debug; fi
	cd cmake-build-debug && \
		cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..

.PHONY: build
build: ./cmake-build-debug/Makefile
	cd cmake-build-debug && make

.PHONY: run
run: ./cmake-build-debug/backbone
	cd cmake-build-debug && ./backbone
