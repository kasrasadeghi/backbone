B-DIR=src/cmake-build-debug

all: build

.PHONY: clean
clean: ${B-DIR}
	rm -rf ${B-DIR}

.PHONY: build
build:
	@(cd src; make)
	ln -sf ${B-DIR}/backbone backbone

test:
	@echo testing parse
	@python3 ktest.py parse
	@echo testing flatten
	@python3 ktest.py flatten
	@echo testing gen
	@python3 ktest.py gen

parse: build
	python3 ktest.py parse
parse\:%: build
	./backbone parse parser-tests/$*.bb

flatten: build
	python3 ktest.py flatten
flatten\:%: build
	./backbone flatten flatten-tests/$*.bb

gen: build
	python3 ktest.py gen
gen\:%: build
	./backbone gen gen-tests/$*.bb

output: build
	python3 ktest.py parse

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
