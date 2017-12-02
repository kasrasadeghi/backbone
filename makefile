DIR=cmake-build-debug

all: prep build

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
	@${DIR}/backbone generateLLVM gen-llvm-tests/$*

test:
	@./test.py

version:
	cmake --version
	make --version
	bash --version
	gcc --version
	clang --version
	python3 --version

help:
	@echo "make COMMAND"
	@echo "COMMANDS:"
	@echo "  version      - check versions"
	@echo "  parse:hi.bb  - parse    hi.bb    in parser-tests"
	@echo "  gen:hello.bb - generate hello.bb in gen-llvm-tests"
