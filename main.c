#include "netinfo.h"

int main(int argc, char **argv)
{
    struct _netinfo_interface iface[100];
    
    int interface_count = netinfo_get_interface_count();
    
    printf("*****************************************************\n");

    netinfo_get_base_config(iface);
    for (int idx = 0; idx < interface_count; idx++) {
        printf("Interface name: %s\n", iface[idx].iface_name);
        printf("IP: %s\n", iface[idx].ipv4);
        printf("Netmask: %s\n", iface[idx].netmask);
        printf("MAC address: %s\n", iface[idx].mac_address);

        // If interface is wireless, print connection information.
        if (iface[idx].net_iface_type == NETINFO_INTERFACE_TYPE_WIRELESS) {
            printf("ESSID: %s\n", iface[idx].netinfo_wireless.essid);
            printf("AP MAC address: %s\n", iface[idx].netinfo_wireless.ap_mac_address);
            printf("Bit rate: %d bps\n", iface[idx].netinfo_wireless.bit_rate);
            printf("Channel: %d\n", iface[idx].netinfo_wireless.channel);
            printf("Frequency: %e hz\n", iface[idx].netinfo_wireless.frequency);
            printf("Quility: %d\n", iface[idx].netinfo_wireless.quility);
            printf("Txpower: %d dBM\n", iface[idx].netinfo_wireless.tx_power);
        }
        printf("\n");
    }

    printf("*****************************************************\n");

    return 0;
}