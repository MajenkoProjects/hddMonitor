#
# Regular cron jobs for the hddmonitor-udpserver package
#
0 4	* * *	root	[ -x /usr/bin/hddmonitor-udpserver_maintenance ] && /usr/bin/hddmonitor-udpserver_maintenance
