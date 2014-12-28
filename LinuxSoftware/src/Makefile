CFLAGS=-Wall -Werror -ggdb3

all: hddMonitorClient hddMonitorServer

hddMonitorClient: hddMonitorClient.o
	$(CC) -o hddMonitorClient hddMonitorClient.o

hddMonitorServer: hddMonitorServer.o
	$(CC) -o hddMonitorServer hddMonitorServer.o

clean:
	rm -f hddMonitorClient hddMonitorServer hddMonitorClient.o hddMonitorServer.o

build: ${BIN}

install: build
	mkdir -p ${DESTDIR}/usr/bin
	mkdir -p ${DESTDIR}/usr/sbin
	mkdir -p ${DESTDIR}/etc/default
	install -m 755 hddMonitorClient ${DESTDIR}/usr/bin
	install -m 755 hddMonitorServer ${DESTDIR}/usr/sbin
	install -m 755 hddmonitor ${DESTDIR}/etc/default
