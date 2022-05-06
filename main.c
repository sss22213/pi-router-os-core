#include "netinfo.h"
#include "wireless_scan.h"

int main(int argc, char **argv)
{
    struct _wireless_scan_netinfo wireless_scan_netinfo;

    struct _netinfo_wireless *ptr_netinfo_interface = NULL;

    wireless_get_scan_result(&wireless_scan_netinfo, "wlan2");
    
    ptr_netinfo_interface = wireless_scan_netinfo.ptr_netinfo_interface_start;
    while (ptr_netinfo_interface < wireless_scan_netinfo.ptr_netinfo_interface_end) {
        
        printf("ESSID: %s, AP_MAC_ADDRESS: %s, FREQENCY: %2.4e, CHANNEL: %d, ENCRPTY: %d, STATUS: %d, ", ptr_netinfo_interface->essid, \
                                                                                ptr_netinfo_interface->ap_mac_address, \
                                                                                ptr_netinfo_interface->frequency,  \
                                                                                ptr_netinfo_interface->channel, \
                                                                                ptr_netinfo_interface->wireless_scan_ap_encrpyt, \
                                                                                ptr_netinfo_interface->wireless_scan_ap_encrpty_status);
        printf("BITRATE:");
        for (int bitrate_idx = 0; bitrate_idx < ptr_netinfo_interface->bit_rate_len; bitrate_idx++) {
            printf("%d,", ptr_netinfo_interface->bit_rate_scanning[bitrate_idx]);
        }
        
        printf("\n");
        
                                                                          
        ptr_netinfo_interface++;
    }
    return 0;
}