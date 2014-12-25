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

Debian / Ubuntu package
=======================

You can now create a debian package (.deb) direct from this directory:

    $ dpkg-buildpackage

Once built the .deb package will be in the parent folder.  Install it with:

    $ sudo dpkg -i hddmonitor-udpclient_1.0.0_amd64.deb

(Replace the filename with the proper one for the current version and your architecture).

An init script is then installed and configured.  The IP to communicate with is configured in the file
/etc/default/hddmonitor

