DIR=cmake-build-debug

all: build

.PHONY: prep
prep:
	@mkdir -p ${DIR}
	(cd ${DIR}; cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..)

.PHONY: build
build: prep
	@cd ${DIR} && make
