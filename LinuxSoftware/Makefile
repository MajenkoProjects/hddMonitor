BIN=UDPClient
OBJS=UDPClient.o
CFLAGS=-Wall -Werror -ggdb3

all: build

clean:
	${MAKE} -C src clean

build:
	${MAKE} -C src all

install: build
	${MAKE} -C src install
