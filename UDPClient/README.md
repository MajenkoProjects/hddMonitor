UDPClient
=========

Building
--------

Just type "make", it should compile.

Running
-------

The program takes just one parameter - the hostname or IP address of the
computer running the UDPServer.  If it is behind a router and that router
is configured to forward the packets on, then this should be the hostname
or IP address of the router.

$ ./UDPClient my.desktop.computer.com

It doesn't (yet) auto-fork, so you can put it in the backgroud with an &:

$ ./UDPClient my.desktop.computer.com &

