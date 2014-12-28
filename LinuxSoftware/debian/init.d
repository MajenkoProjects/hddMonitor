#! /bin/bash
### BEGIN INIT INFO
# Provides: hddmonitor
# Required-Start: ${all}
# Required-Stop: ${all}
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: hddMonitor
# Description: hddMonitor
### END INIT INFO

PATH=/sbin:/usr/sbin:/bin:/usr/bin

. /lib/lsb/init-functions

. /etc/default/hddmonitor

case "$1" in
    start)

        if [ ! -z ${device} ]; then
            log_daemon_msg "Starting HDD Monitor server" "${device}" || true
            if hddMonitorServer ${device}; then
                log_end_msg 0 || true
            else
                log_end_msg 1 || true
            fi
        fi

        UNIT=0
        DU=drive${UNIT}
        IU=id${UNIT}
        PU=ip${UNIT}
        while [ ! -z "${!DU}" ]; do
            log_daemon_msg "Starting HDD Monitor client" "${!DU}" || true
            if hddMonitorClient ${!DU} ${!IU} ${!PU}; then
                log_end_msg 0 || true
            else
                log_end_msg 1 || true
            fi
            ((++UNIT))
            DU=drive${UNIT}
            IU=id${UNIT}
            PU=ip${UNIT}
        done
        ;;  

    restart|reload|force-reload)
        $0 stop
        $0 start
        ;;
    stop)

        if [ ! -z ${device} ]; then
            log_daemon_msg "Stopping HDD Monitor server" "${device}" || true
            if killall -9 hddMonitorServer; then
                log_end_msg 0 || true
            else
                log_end_msg 1 || true
            fi
        fi

        UNIT=0
        DU=drive${UNIT}
        IU=id${UNIT}
        PU=ip${UNIT}
        while [ ! -z "${!DU}" ]; do
            log_daemon_msg "Stopping HDD Monitor client" "${!DU}" || true
            PID=`ps ax | grep "hddMonitorClient ${!DU} ${!IU} ${!PU}" | grep -v grep | awk '{print $1}'`
            if [ ! -z "${PID}" ]; then
                if kill -9 ${PID}; then
                    log_end_msg 0 || true
                else
                    log_end_msg 1 || true
                fi
            else
                log_end_msg 1 || true
            fi

            ((++UNIT))
            DU=drive${UNIT}
            IU=id${UNIT}
            PU=ip${UNIT}
        done
        ;;
    *)
        echo "Usage: $0 start|stop" >&2
        exit 3
        ;;
esac

exit 0
