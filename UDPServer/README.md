UDPServer
=========

Building
--------

Just type "make", it should compile.

Running
-------

The program takes just one parameter - the Arduino's serial device.

    $ ./UDPServer /dev/ttyACM0

Debian / Ubuntu package
=======================

You can now create a debian package (.deb) direct from this directory:

    $ dpkg-buildpackage

Once built the .deb package will be in the parent folder.  Install it with:

    $ sudo dpkg -i hddmonitor-udpserver_1.0.0_amd64.deb

(Replace the filename with the proper one for the current version and your architecture).

An init script is then installed and configured.  The device to communicate with is configured in the file
/etc/default/hddmonitor

It can be quite useful to create a symlink to your board to give it a known static device node.  I use a
udev rules file containing:

    SUBSYSTEMS=="usb", ATTRS{product}=="Arduino Uno", KERNEL=="ttyACM*", SYMLINK+="board/arduino/uno", MODE="0666", GROUP="plugdev"

That creates a symlink to the correct /dev/ttyACM device as /dev/board/arduino/uno regardless of what other CDC/ACM devices there are.

