B-DIR=src/cmake-build-debug

all: clean build test

.PHONY: clean
clean: 
	-rm -rf ${B-DIR}
	(cd gen-tests; make clean)

.PHONY: build
build:
	@(cd src; make --no-print-directory)
	ln -sf ${B-DIR}/backbone backbone

test:
	@python3 ktest.py parse
	@python3 ktest.py blockify
	@python3 ktest.py normalize
	@python3 ktest.py qualify
	@python3 ktest.py gen
	@python3 ktest.py output

parse\:%: build
	./backbone parse parser-tests/$*.bb

normalize\:%: build
	./backbone normalize normalize-tests/$*.bb

qualify\:%: build
	./backbone qualify qualify-tests/$*.bb

gen\:%: build
	./backbone gen gen-tests/$*.bb

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

%: build %.ktest
	python3 ktest.py $*