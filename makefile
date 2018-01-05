B-DIR=src/cmake-build-debug

all: build test

.PHONY: clean
clean: 
	-rm -rf ${B-DIR}
	(cd gen-tests; make clean)

.PHONY: build
build:
	@(cd src; make --no-print-directory)
	ln -sf ${B-DIR}/backbone backbone

test:
	@python3 ktest.py

parse: build
	python3 ktest.py parse
parse\:%: build
	./backbone parse parser-tests/$*.bb

flatten: build
	python3 ktest.py flatten
flatten\:%: build
	./backbone flatten flatten-tests/$*.bb

qualify: build
	python3 ktest.py qualify
qualify\:%: build
	./backbone qualify qualify-tests/$*.bb

gen: build
	python3 ktest.py gen
gen\:%: build
	./backbone gen gen-tests/$*.bb

output: build
	python3 ktest.py output

help:
	@echo "  version   - check versions"
	@echo "  parse:hi  - parse    hi    in parser-tests"
	@echo "  gen:hello - generate hello in gen-tests"

version:
	cmake --version
	make --version
	bash --version
	clang --version
	python3 --version

docker-run: clean
	docker build -t bb .
	(cd src; rm -rf cmake-build-debug)
	docker run -it -v $(shell pwd):/project bb

docker-clean:
	-docker ps -a -q | xargs docker rm
	-docker images | grep "^<none>" | awk "{print $3}" | xargs docker rmi -f
