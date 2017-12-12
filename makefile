B-DIR=src/cmake-build-debug

all: prep build

.PHONY: build
build:
	@(cd src; make)

parse\:%: build
	@./backbone parse parser-tests/$*.bb

gen\:%: build
	@./backbone generateLLVM gen-llvm-tests/$*.bb

test:
	@./test.py

help:
	@echo "  version   - check versions"
	@echo "  parse:hi  - parse    hi    in parser-tests"
	@echo "  gen:hello - generate hello in gen-llvm-tests"

version:
	cmake --version
	make --version
	bash --version
	gcc --version
	clang --version
	python3 --version
