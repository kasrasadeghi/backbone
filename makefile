B-DIR=src/cmake-build-debug

all: build

.PHONY: build
build:
	@(cd src; make)
	ln -sf ${B-DIR}/backbone backbone

gen: build
	python3 ktest.py gen

flatten: build
	python3 ktest.py flatten

gen\:%: build
	./backbone gen gen-tests/$*.bb

flatten\:%: build
	./backbone flatten gen-tests/$*.bb

output: build
	python3 ktest.py parse

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
