B-DIR=src/cmake-build-debug

all: build

.PHONY: build
build:
	@(cd src; make)
	ln -sf ${B-DIR}/backbone backbone

parse\:%: build
	./backbone parse parser-tests/$*.bb

flatten: build
	python3 ktest.py flatten
flatten\:%: build
	./backbone flatten gen-tests/$*.bb

gen: build
	python3 ktest.py gen
gen\:%: build
	./backbone gen gen-tests/$*.bb

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

docker-run:
	docker build -t bb .
	docker run -it bb

docker-clean:
	-docker ps -a -q | xargs docker rm
	-docker images | grep "^<none>" | awk "{print $3}" | xargs docker rmi -f