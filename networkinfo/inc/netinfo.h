#ifndef __NETINFO_H__
#define __NETINFO_H__
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <net/ethernet.h>
#include "wireless.h"

#define ESSID_LENGTH    100
#define IFACE_NAME_LENGTH   100
#define IFACE_MAXIMUN_LENGTH    100
#define MAC_ADDRESS_MAXIMUM_LENGTH 20
#define SCAN_BITRATE_MAXIMUM_LENGTH 25

typedef enum {
   NETINFO_INTERFACE_TYPE_ENTERNET,
   NETINFO_INTERFACE_TYPE_WIRELESS, 
   NETINFO_INTERFACE_TYPE_UNKNOWN,
} NETINFO_INTERFACE_TYPE;

typedef enum {
    NETINFO_LINK_DOWN,
    NETINFO_LINK_UP,
} NETINFO_LINK_STATUS;

typedef enum {
    NETINFO_WIRELESS_AP_MODE,
    NETINFO_WIRELESS_STATION_MODE,
    NETINFO_WIRELESS_MIX_MODE,
} NETINFO_WIRELESS_MODE;

struct _netinfo_wireless {
    char essid[ESSID_LENGTH];
    float frequency; // hz
    int channel;
    NETINFO_WIRELESS_MODE netinfo_wireless_info;
    int bit_rate; // bps
    int tx_power; // dBm
    int quility; // percent
    char ap_mac_address[MAC_ADDRESS_MAXIMUM_LENGTH];
    // For wireless scanning
    int bit_rate_scanning[SCAN_BITRATE_MAXIMUM_LENGTH];
    int bit_rate_len;
};

struct _netinfo_interface {
    char iface_name[IFACE_NAME_LENGTH];
    NETINFO_LINK_STATUS netinfo_link_status;
    NETINFO_INTERFACE_TYPE net_iface_type;
    char ipv4[INET_ADDRSTRLEN];
    char netmask[INET_ADDRSTRLEN];
    char gateway[INET_ADDRSTRLEN];
    char mac_address[MAC_ADDRESS_MAXIMUM_LENGTH];
    struct _netinfo_wireless netinfo_wireless;
};


bool netinfo_get_base_config(struct _netinfo_interface*);

bool netinfo_list_all_interface_name(char (*)[IFACE_NAME_LENGTH]);

bool netinfo_get_wireless_base_config(const char *, struct _netinfo_interface*);

uint8_t netinfo_get_interface_count(void);

void ssid_scanning(void);

#endif