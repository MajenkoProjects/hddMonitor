BIN=UDPServer
OBJS=UDPServer.o
CFLAGS=-Wall -Werror

${BIN}: ${OBJS}
	$(CC) -o ${BIN} ${OBJS}

clean:
	rm -f ${BIN} ${OBJS}

build: ${BIN}

install: build
	mkdir -p ${DESTDIR}/usr/sbin
	mkdir -p ${DESTDIR}/etc/default
	install -m 755 ${BIN} ${DESTDIR}/usr/sbin
	install -m 644 hddmonitor ${DESTDIR}/etc/default
