hddMonitor
==========

* Hard disk activity monitor for remote servers.

Now, I don't know about you, but I like to know what my computers are
doing.  I was once forced to use a Mac for a while, and while it was
a well-built machine, it really got on my nerves that there wasn't a
hard drive light on it.

When the computer grinds to a halt it's nice to know that there is
actually a reason for it and the hard drive is being heavilly accessed.
Without a hard drive light you can't get that kind of feedback.

The same holds true for a remote server.  If it's going slow you want to
know if it's the hard drive that's being thrashed, and how often.  And what
better way than with a hard drive light.

BUT, you can't be sat infront of a server in a data center the other side
of the country watching the lights flash, so this little project was born.

Using this small set of programs, and an Arduino with a TLC59116 I2C LED
driver board with RGB LEDs on it (or you could modify it to just use the
Arduino's digital IO pins if you wanted), you can get realtime disk activity
lights for your remote computers right on your desk.

The system consists of three parts:

* hddMonitorClient

This small program monitors the disk in a computer.  It does so by watching
the contents of /sys/block/<drive>/stat.  Every time there is a change in the
read or write count (with a granularity of 100ms) it crafts a small UDP packet
and fires it at your computer (assuming it has access of course - you may want
a VPN, or to configure your router to forward the UDP packets to your
computer).

* hddMonitorServer

This small program receives the UDP packets, decodes them, and converts them
into a serial data stream consisting of three parts - the IP address of the
sending station, the delta read count, and the delta write count.  These are
then passed on to an Arduino (I use an Uno) for further processing.

* LEDDriver

This is the Arduino sketch.  It receives the data stream from UDPServer
and decides what to do with the data.  It matches the IP address with known
valid addresses, and associates them with a set of RGB LEDs.  The LEDs
are then flashed a different colour (with a fade-out) depending on the
magnitude of the value.  The colours are in blocks of 255, and are, in
ascending order:

Red, Yellow, White, Cyan, Blue.

