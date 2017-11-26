DIR=cmake-build-debug

all: prep build run\:some.bb

.PHONY: prep
prep:
	if [ ! -d ${DIR} ]; then mkdir ${DIR}; fi
	cd ${DIR} && \
		cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..

.PHONY: build
build: ${DIR}/Makefile
	cd ${DIR} && make

.PHONY: run
run\:%: ${DIR}/backbone
	cd ${DIR} && ./backbone ../examples/$*
