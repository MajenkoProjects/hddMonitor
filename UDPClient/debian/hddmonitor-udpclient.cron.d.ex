#
# Regular cron jobs for the hddmonitor-udpclient package
#
0 4	* * *	root	[ -x /usr/bin/hddmonitor-udpclient_maintenance ] && /usr/bin/hddmonitor-udpclient_maintenance
