UDPServer
=========

Building
--------

Just type "make", it should compile.

Running
-------

The program takes just one parameter - the Arduino's serial device.

$ ./UDPServer /dev/ttyACM0

It doesn't (yet) auto-fork, so you can put it in the backgroud with an &:

$ ./UDPServer /dev/ttYACM0 &

