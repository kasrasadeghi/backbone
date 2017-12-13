DIR=cmake-build-debug

all: build

${DIR}/Makefile:
	@mkdir -p ${DIR}
	(cd ${DIR}; cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..)

.PHONY: build
build: ${DIR}/Makefile
	@cd ${DIR} && make
