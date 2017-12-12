DIR=cmake-build-debug

all: prep build

.PHONY: prep
prep:
	@mkdir -p ${DIR}
	(cd ${DIR}; cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..)

.PHONY: build
build: ${DIR}/Makefile
	@cd ${DIR} && make
