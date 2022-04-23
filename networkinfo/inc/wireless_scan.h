#ifndef __WIRELESS_SCAN_H__
#define __WIRELESS_SCAN_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <net/ethernet.h>
#include "netinfo.h"
#include "wireless.h"

#define MAXIMUM_STREAM_PACKAGE_COUNT    100
#define EVENT_CMD_LENGTH	2
#define MAXIMUM_WIRELSS_SCAN_EVENT_LENGTH	4096
#define MAXIMUM_WIRLESS_CELL	100

struct _stream_package {
    struct iw_ioctl_description *iwioctl_description;
    union iwreq_data iwreq;
};

struct _wirelss_scan_event {
	struct iw_event i_event;
	char buf[MAXIMUM_STREAM_PACKAGE_COUNT];
};

struct _wireless_scan_netinfo {
	int cell_len;
	struct _netinfo_wireless *ptr_netinfo_interface_start;
	struct _netinfo_wireless *ptr_netinfo_interface_end;
	struct _netinfo_wireless netinfo_interface[MAXIMUM_WIRLESS_CELL];
};

bool wireless_get_scan_result(struct _wireless_scan_netinfo *, const char *);

#endif