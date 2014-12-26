#! /bin/sh
### BEGIN INIT INFO
# Provides: hddmonitor-udpclient
# Required-Start: ${all}
# Required-Stop: ${all}
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: hddMonitor UDP Client
# Description: hddMonitor UDP Client
### END INIT INFO

PATH=/sbin:/usr/sbin:/bin:/usr/bin

. /lib/lsb/init-functions

. /etc/default/hddmonitor

case "$1" in
    start|restart|reload|force-reload)
        log_daemon_msg "Starting HDD Monitor client" "${drive}" || true
        if UDPClient ${drive} ${id} ${ip}; then
            log_end_msg 0 || true
        else
            log_end_msg 1 || true
        fi
        ;;  
    restart|reload|force-reload)
        log_daemon_msg "Restarting HDD Monitor client" "${drive}" || true
        killall UDPClient
        sleep 1
        if UDPClient ${drive} ${id} ${ip}; then
            log_end_msg 0 || true
        else
            log_end_msg 1 || true
        fi
        log_end_msg 1 || true
        ;;
    stop)
        log_daemon_msg "Stopping HDD Monitor client" "${drive}" || true
        killall UDPClient
        log_end_msg 0 || true
        ;;
    *)
        echo "Usage: $0 start|stop" >&2
        exit 3
        ;;
esac

exit 0
