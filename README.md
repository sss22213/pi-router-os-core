# pi-router-os-core

This repository contains network interface information by ioctl, netdevice(7), wireless.h.

## Test environment:
1. Raspberry pi x64
2. pi-router-os [pi-router-os](https://github.com/sss22213/pi-router-os) (Modify from OpenWRT)

## Test program:
```bash=
make
./main
```

## API
```Clike=
// Get interface number.
uint8_t netinfo_get_interface_count(void)

/**
 * @brief Get all of interface name.
 * IFACE_NAME_LENGTH is default set 100. 
 * 
 * @return true Get success
 * @return false Get failed
 */
bool netinfo_list_all_interface_name(char (*)[IFACE_NAME_LENGTH]);

/**
 * @brief Get wireless interface information.
 * 
 * @param iface_name interface name
 * @param iface pointer to structure of _netinfo_interface
 * @return true Get success
 * @return false Get failed
 */
bool netinfo_get_wireless_base_config(const char *iface_name, struct _netinfo_interface *iface);

/**
 * @brief Get all of interface information. 
 * 
 * @param iface pointer to structure of _netinfo_interface
 * @return true Get success
 * @return false Get failed
 */
 bool netinfo_get_base_config(struct _netinfo_interface *iface);

// Structure of wireless.
struct _netinfo_wireless {
    // ESSID
    char essid[ESSID_LENGTH];
    
    // Frequency
    float frequency; // hz
    
    // Channel
    int channel;
    
    // UNUSED
    NETINFO_WIRELESS_MODE netinfo_wireless_info;
    
    // Bit rate
    int bit_rate; // bps
    
    // Txpower
    int tx_power; // dBm
    
    // Quility (%)
    int quility; // percent
    
    // AP MAC address of connection 
    char ap_mac_address[MAC_ADDRESS_MAXIMUM_LENGTH];
};

// Structure of network interface.
struct _netinfo_interface {
    // interface name
    char iface_name[IFACE_NAME_LENGTH];
    
    // network link status: NETINFO_LINK_DOWN, NETINFO_LINK_UP.
    NETINFO_LINK_STATUS netinfo_link_status;
   
    // network interface type: NETINFO_INTERFACE_TYPE_ENTERNET, NETINFO_INTERFACE_TYPE_WIRELESS, NETINFO_INTERFACE_TYPE_UNKNOWN
    NETINFO_INTERFACE_TYPE net_iface_type;
    
    // IPV4
    char ipv4[INET_ADDRSTRLEN];
    
    // Netmask
    char netmask[INET_ADDRSTRLEN];
    
    // UNUSED
    char gateway[INET_ADDRSTRLEN];
    
    // PHY mac address
    char mac_address[MAC_ADDRESS_MAXIMUM_LENGTH];
    
    // Structure of _netinfo_wireless
    struct _netinfo_wireless netinfo_wireless;
};
```

![This is an image](https://i.imgur.com/l3tpCNc.png)
