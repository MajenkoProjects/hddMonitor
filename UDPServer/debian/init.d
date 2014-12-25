#! /bin/sh
### BEGIN INIT INFO
# Provides: hddmonitor-udpserver
# Required-Start: ${all}
# Required-Stop: ${all}
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: hddMonitor UDP Server
# Description: hddMonitor UDP Server
### END INIT INFO

PATH=/sbin:/usr/sbin:/bin:/usr/bin

. /lib/lsb/init-functions

. /etc/default/hddmonitor

case "$1" in
    start|restart|reload|force-reload)
        log_daemon_msg "Starting HDD Momitor server" "${device}" || true
        if UDPServer "${device}"; then
            log_end_msg 0 || true
        else
            log_end_msg 1 || true
        fi
        ;;  
    restart|reload|force-reload)
        log_daemon_msg "Restarting HDD Momitor server" "${device}" || true
        killall UDPServer
        sleep 1
        if UDPServer "${device}"; then
            log_end_msg 0 || true
        else
            log_end_msg 1 || true
        fi
        log_end_msg 1 || true
        ;;
    stop)
        log_daemon_msg "Stopping HDD Momitor server" "${device}" || true
        killall UDPServer
        log_end_msg 0 || true
        ;;
    *)
        echo "Usage: $0 start|stop" >&2
        exit 3
        ;;
esac

exit 0
