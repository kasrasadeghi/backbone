FROM ubuntu:17.10

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    cmake \
    clang \
    valgrind \
    python3

COPY . /project

WORKDIR /project

CMD bash
