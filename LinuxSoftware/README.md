UDPClient
=========

Building
--------

Just type "make", it should compile.

Running
-------

The client program takes three parameters: The disk name, the channel ID, 
and the hostname or IP of the computer running the UDPServer.  If it is 
behind a router and that router is configured to forward the packets on, 
then this should be the hostname or IP address of the router.

$ ./hddMonitorClient sda 0 my.desktop.computer.com

The server takes just one parameter - the device to communicate with:

$ ./hddMonitorServer /dev/ttyACM0

Debian / Ubuntu package
=======================

You can now create a debian package (.deb) direct from this directory:

    $ dpkg-buildpackage

Once built the .deb package will be in the parent folder.  Install it with:

    $ sudo dpkg -i hddmonitor_1.0.2_amd64.deb

(Replace the filename with the proper one for the current version and your architecture).

An init script is then installed and configured.  The drive, channel and 
IP etc to communicate with are configured in the file /etc/default/hddmonitor

It also installs an init script.  Start and stop the daemons with:

    $ sudo /etc/init.d/hddmonitor start

and 

    $ sudo /etc/init.d/hddmonitor stop
