B-DIR=src/cmake-build-debug

all: build

.PHONY: build
build:
	@(cd src; make)
	ln -sf ${B-DIR}/backbone backbone

gen: build
	python3 test_gen.py

output: build
	python3 test_output.py

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
