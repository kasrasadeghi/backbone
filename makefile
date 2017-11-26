DIR=cmake-build-debug

all: prep build parse\:string.bb

.PHONY: prep
prep:
	@mkdir -p ${DIR}
	(cd ${DIR}; cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..)

.PHONY: build
build: ${DIR}/Makefile
	@cd ${DIR} && make &> /dev/null

.PHONY: run
parse\:%: build
	@cd ${DIR} && ./backbone parse ../parser-tests/$*

test:
	ls
	./test.py