FROM ubuntu:22.04
RUN apt-get update -y && apt-get install build-essential cmake libomp-dev git -y
COPY . /simulator
WORKDIR /simulator
RUN mkdir build && cd build && cmake .. && make
ENTRYPOINT /bin/bash
