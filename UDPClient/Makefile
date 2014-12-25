BIN=UDPClient
OBJS=UDPClient.o
CFLAGS=-Wall -Werror -ggdb3

${BIN}: ${OBJS}
	$(CC) -o ${BIN} ${OBJS} -ggdb3

clean:
	rm -f ${BIN} ${OBJS}

build: ${BIN}

install: build
	mkdir -p ${DESTDIR}/usr/bin
	mkdir -p ${DESTDIR}/etc/default
	install -m 755 ${BIN} ${DESTDIR}/usr/bin
	install -m 755 hddmonitor ${DESTDIR}/etc/default
