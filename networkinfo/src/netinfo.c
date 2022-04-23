#include "netinfo.h"

/**
 * @brief Check if interface exist.
 * 
 * @param iface_name 
 * @return true 
 * @return false 
 */
static inline bool _interface_exist(const char *iface_name)
{
    struct ifreq ifr;
    int ret = 0;
    int fd = 0;

    strcpy(ifr.ifr_name, iface_name);

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    ret = ioctl(fd, SIOCGIFFLAGS, ifr);

    close(fd);

    return ret==0?true:false;
}


/**
 * @brief Get interface type (ENTERNET, WIRELESS, UNKNOWN)
 * 
 * @param iface_name interface name
 * @return NETINFO_INTERFACE_TYPE 
 */
static inline NETINFO_INTERFACE_TYPE _netinfo_interface_type(const char *iface_name)
{
    struct iwreq iw_req;

    int fd = 0;

    int ret = 0;

    if (_interface_exist(iface_name) == false) {
        return NETINFO_INTERFACE_TYPE_UNKNOWN;
    }

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    strcpy(iw_req.ifr_ifrn.ifrn_name, iface_name);

    ret = ioctl(fd, SIOCGIWNAME, &iw_req);

    close(fd);

    if (ret == -1) {
        return NETINFO_INTERFACE_TYPE_ENTERNET;
    } else {
        return NETINFO_INTERFACE_TYPE_WIRELESS;
    }
}

uint8_t netinfo_get_interface_count(void)
{
    int fd = 0;

    int ret = 0;

    struct ifconf ifc;

    struct ifreq ifr[IFACE_MAXIMUN_LENGTH];

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    ifc.ifc_len = sizeof(struct ifreq)*IFACE_MAXIMUN_LENGTH;
    ifc.ifc_buf = (char*)ifr;
    
    ret = ioctl(fd, SIOCGIFCONF, (char *)&ifc);

    close(fd);

    if (ret == -1) {
        return 0;
    }

    return ifc.ifc_len/sizeof(struct ifreq);
}

/**
 * @brief Get all of interface name.
 * 
 * @return true 
 * @return false 
 */
bool netinfo_list_all_interface_name(char (*ary)[IFACE_NAME_LENGTH])
{
    int fd = 0;

    int ret = 0;

    struct ifreq ifr[IFACE_MAXIMUN_LENGTH];

    struct ifconf ifc;
    ifc.ifc_len = IFACE_MAXIMUN_LENGTH * sizeof(struct ifreq);
    ifc.ifc_buf = (char*)(ifr);

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    ret = ioctl(fd, SIOCGIFCONF, (char*)&ifc);

    close(fd);

    if (ret == -1) {
        return false;
    }

    for (int iface_idx = 0; iface_idx < ifc.ifc_len / sizeof(struct ifreq); iface_idx++) {
        strncpy(ary[iface_idx], ifr[iface_idx].ifr_name, sizeof(char)*IFACE_NAME_LENGTH);
    }

    return true;
}

/**
 * @brief 
 * 
 * @param iface_name 
 * @param iface 
 * @return true 
 * @return false 
 */
bool netinfo_get_wireless_base_config(const char *iface_name, struct _netinfo_interface *iface)
{

    if (_netinfo_interface_type(iface_name) != NETINFO_INTERFACE_TYPE_WIRELESS) {
        return false;
    } 

    struct iwreq iw_req;

    int fd = 0;
    int ret = 0;

    uint8_t buffer[ESSID_LENGTH] = {0};

    struct iw_statistics iw_stat;

    strcpy(iw_req.ifr_ifrn.ifrn_name, iface_name); 

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    // Get ESSID
    iw_req.u.essid.pointer = buffer;
    iw_req.u.essid.length = sizeof(char)*ESSID_LENGTH;

    ret = ioctl(fd, SIOCGIWESSID, &iw_req);
    if (ret == -1) {
        return false;
    }
    strncpy(iface->netinfo_wireless.essid, (char*)iw_req.u.essid.pointer, sizeof(char)*ESSID_LENGTH);

    // Get frequency/channel
    ret = ioctl(fd, SIOCGIWFREQ, &iw_req);
    if (ret == -1) {
        return false;
    }
    iface->netinfo_wireless.frequency = (float)iw_req.u.freq.m / 1000.0;
    iface->netinfo_wireless.channel = iw_req.u.freq.e;

    // Get bitrate
    ret = ioctl(fd, SIOCGIWRATE, &iw_req);
    if (ret == -1) {
        return false;
    }
    iface->netinfo_wireless.bit_rate = iw_req.u.bitrate.value;

    // Get txpower
    ret = ioctl(fd, SIOCGIWTXPOW, &iw_req);
    if (ret == -1) {
        return false;
    }
    iface->netinfo_wireless.tx_power = iw_req.u.txpower.value;

    // Get ap address
    ret = ioctl(fd, SIOCGIWAP, &iw_req);
    if (ret == -1) {
        return false;
    }

    for (int ap_addr_idx = 0; ap_addr_idx < 6; ap_addr_idx++) {
        sprintf(iface->netinfo_wireless.ap_mac_address + 3*ap_addr_idx, "%02x:", iw_req.u.ap_addr.sa_data[ap_addr_idx]);
    }
    *(iface->netinfo_wireless.ap_mac_address + 17) = '\0';

    // Get wifi connect quility
    iw_req.u.data.pointer = (char*)&iw_stat;
    iw_req.u.data.length = sizeof(struct iw_statistics);
    iw_req.u.data.flags = 1;
    ret = ioctl(fd, SIOCGIWSTATS, &iw_req);
    if (ret == -1) {
        return false;
    }

    iface->netinfo_wireless.quility = iw_stat.qual.qual;
    
    close(fd);

    return true;
}

/**
 * @brief Get all of interface information. 
 * 
 * @param iface pointer to structure of _netinfo_interface
 * @return true
 * @return false 
 */
bool netinfo_get_base_config(struct _netinfo_interface *iface)
{
    int fd = 0;

    int ret = 0;

    struct ifreq ifr[IFACE_MAXIMUN_LENGTH];

    char ipaddr[INET_ADDRSTRLEN] = {'\0'};

    char netmask[INET_ADDRSTRLEN] = {'\0'};

    char macaddress[MAC_ADDRESS_MAXIMUM_LENGTH] = {'\0'};

    struct ifconf ifc;
    ifc.ifc_len = IFACE_MAXIMUN_LENGTH * sizeof(struct ifreq);;
    ifc.ifc_buf = (char*)(ifr);

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    ret = ioctl(fd, SIOCGIFCONF, (char *)&ifc);
    if (ret == -1) {
        close(fd);
        return false;
    }

    // Get IP and interface name
    for (int iface_idx = 0; iface_idx < ifc.ifc_len / sizeof(struct ifreq); iface_idx++) {
        // Interface name
        strncpy(iface[iface_idx].iface_name, ifr[iface_idx].ifr_name, sizeof(char)*IFACE_NAME_LENGTH);
        
        // IP address
        inet_ntop(AF_INET, 
        &(((struct sockaddr_in *)&(ifr[iface_idx].ifr_addr))->sin_addr),
        ipaddr, INET_ADDRSTRLEN);

        strncpy(iface[iface_idx].ipv4, ipaddr, sizeof(char)*INET_ADDRSTRLEN);

        // Get netmask
        ret = ioctl(fd, SIOCGIFNETMASK, &ifr[iface_idx]);
        if (ret == -1) {
            close(fd);
            return false;
        }

        inet_ntop(AF_INET, 
        &(((struct sockaddr_in *)&(ifr[iface_idx].ifr_addr))->sin_addr),
        netmask, INET_ADDRSTRLEN);

        strncpy(iface[iface_idx].netmask, netmask, sizeof(char)*INET_ADDRSTRLEN);

        // Get MAC address
        ret = ioctl(fd, SIOCGIFHWADDR, &ifr[iface_idx]);
        if (ret == -1) {
            close(fd);
            return false;
        }

        for (int mac_addr_idx = 0; mac_addr_idx < 6 ; mac_addr_idx++) {
            sprintf(macaddress + 3 * mac_addr_idx, "%02x:", ifr[iface_idx].ifr_hwaddr.sa_data[mac_addr_idx]);
        }

        // Remove tail of ":"
        *(macaddress + 17) = '\0';

        strncpy(iface[iface_idx].mac_address, macaddress, sizeof(char)*MAC_ADDRESS_MAXIMUM_LENGTH);

        // Get link status
        ret = ioctl(fd, SIOCGIFFLAGS, &ifr[iface_idx]);
        if (ret == -1) {
            close(fd);
            return false;
        }

        if (ifr[iface_idx].ifr_flags & IFF_UP) {
            iface[iface_idx].netinfo_link_status = NETINFO_LINK_UP;
        } else {
            iface[iface_idx].netinfo_link_status = NETINFO_LINK_DOWN;
        }

        // Get interface type
        iface[iface_idx].net_iface_type = _netinfo_interface_type(iface[iface_idx].iface_name);

        netinfo_get_wireless_base_config(ifr[iface_idx].ifr_name, &iface[iface_idx]);
    }

    close(fd);

    return true;
}

void ssid_scanning()
{
    int fd = 0;

    int ret = 0;

    uint8_t buf[IW_SCAN_MAX_DATA*10] = {0};

    struct iwreq iw_req;

    struct iw_scan_req iwscan_req;
    memset(&iwscan_req, 0, sizeof(struct iw_scan_req));

    iw_req.u.data.pointer = (caddr_t)&iwscan_req;
    iw_req.u.data.length = sizeof(iwscan_req);

    // Initial scan
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    strncpy(iw_req.ifr_name, "wlan2", IFNAMSIZ);

    ret = ioctl(fd, SIOCSIWSCAN, &iw_req);

    printf("%d\n", ret);

    sleep(5);
    iw_req.u.data.pointer = buf;
	iw_req.u.data.flags = 0;
	iw_req.u.data.length = sizeof(uint8_t) * IW_SCAN_MAX_DATA * 10;
    strncpy(iw_req.ifr_name, "wlan2", IFNAMSIZ);
    
    ret = ioctl(fd, SIOCGIWSCAN, &iw_req);

    /*
    for(int i = 1; i < iw_req.u.data.length; i++)
	    printf(":%02X", buf[i]);
    */

    printf("%d\n", ret);

    close(fd);
}