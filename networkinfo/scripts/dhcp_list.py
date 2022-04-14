import os

# Only use on OpenWRT
class dhcp_list:
    def __init__(self):
        pass
    
    def get_dhcp_list(self):
        fd = open('/tmp/dhcp.leases', 'r')
        dhcp_item_list = []
        
        for line in fd:
            connected_information = line.split(" ")
            dhcp_item = {"timestamp":0, "mac_address":"", "ip":"", "name":""}
            dhcp_item["timestamp"] = connected_information[0]
            dhcp_item["mac_address"] = connected_information[1]
            dhcp_item["ip"] = connected_information[2]
            dhcp_item["name"] = connected_information[3]
            dhcp_item_list.append(dhcp_item)
        fd.close()
        return dhcp_item_list

if __name__ == '__main__':
    d_list = dhcp_list()
    print(d_list.get_dhcp_list())