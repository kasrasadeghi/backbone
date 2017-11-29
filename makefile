DIR=cmake-build-debug

all: prep build parse\:string.bb

.PHONY: prep
prep:
	@mkdir -p ${DIR}
	(cd ${DIR}; cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..)

.PHONY: build
build: ${DIR}/Makefile
	@cd ${DIR} && make

.PHONY: parse
parse\:%: build
	@cd ${DIR} && ./backbone parse ../parser-tests/$*

gen\:%: build
	@{DIR}/backbone generateLLVM parser-tests/$*

test:
	@./test.py

version:
	cmake --version
	make --version
	bash --version
	gcc --version
	clang --version
