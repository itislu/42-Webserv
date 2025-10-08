FROM ubuntu:latest

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    software-properties-common \
	&& apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

RUN add-apt-repository ppa:bkryza/clang-uml \
	&& apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
	clang-uml \
	&& apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

WORKDIR /work
