import socket
import struct
import array
import fcntl
import sys
import time

# System call requirement
SIOMIN = 0x8B00
SIOMAX = 0x8C09	
SIOCSIWSCAN = 0x8B18
SIOCGIWSCAN = 0x8B19
SIOCGIWAP = 0x8B15
SIOCGIWESSID = 0x8B1B
SIOCGIWFREQ = 0x8B05
SIOCGIWRATE = 0x8B21
PEDDING_ZERO = 4
IW_EV_LCP_PK_LEN = 4

# Sockaddr ip location
SOCKADDR_OFFSET = 2

# Command length
CMD_LEN = 2

# LENGTH length
LEN_LEN = 2

class wireless_event:
    cmd = None
    data_len = None
    data = None

class wireless_scan:
    def __init__(self):
        self.fd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def scan(self, iface):
        wreq_buff = array.array('B')
        for wreq_buff_idx in range(100):
            wreq_buff.append(0)
        length = 100
        flag = 0
        input_buffer = struct.pack('16sLLL', bytes(iface, 'utf8'), wreq_buff.buffer_info()[0], length, flag)
        output_buffer = fcntl.ioctl(self.fd.fileno(), SIOCSIWSCAN, input_buffer)

        time.sleep(5)

        scan_buf = array.array('B')
        for scan_buf_idx in range(4096*2):
            scan_buf.append(0)
        length = 4096*2
        flag = 0

        input_buffer = struct.pack('16sLLL', bytes(iface, 'utf8'), scan_buf.buffer_info()[0], length, flag)
        output_buffer = fcntl.ioctl(self.fd.fileno(), SIOCGIWSCAN, input_buffer)
        wireless_scan_size = struct.unpack('16sLLL', output_buffer)[2]
        scan_buf_byte = scan_buf.tobytes()

        event_cmd = scan_buf_byte[2] | (scan_buf_byte[3] << 8)
        event_len = scan_buf_byte[0]
        event_value = []
        next_search_idx = 0

        wireless_event_list = []
        
        search_flag = True
        while search_flag:
            if next_search_idx >= wireless_scan_size-IW_EV_LCP_PK_LEN:
                search_flag = False
            
            event_cmd = scan_buf_byte[next_search_idx+2] | (scan_buf_byte[next_search_idx+3] << 8)
            
            event_len = scan_buf_byte[next_search_idx]
            if event_len <= IW_EV_LCP_PK_LEN:
                next_search_idx = next_search_idx + 1
                continue

            if (event_cmd < SIOMIN) or (event_cmd > SIOMAX):
                next_search_idx = next_search_idx + 1
                continue

            wire_event = wireless_event()
            wire_event.cmd = event_cmd
            wire_event.data_len = event_len
            wire_event.data = scan_buf_byte[(next_search_idx+CMD_LEN+LEN_LEN+PEDDING_ZERO):(next_search_idx+event_len)]
            wireless_event_list.append(wire_event)

            next_search_idx = next_search_idx + event_len
        return wireless_event_list

    def extract(self, wireless_event_list):
        wireless_scan_result = []
        wirless_scan_event = {}
        for event in wireless_event_list:
            if event.cmd == SIOCGIWAP:
                wireless_scan_result.append(wirless_scan_event)
                # Update event
                wirless_scan_event = {"BITRATE":[]}
                AP_addr = ""
                for event_idx in range(2, 8):
                    AP_addr = AP_addr + ":" + str(hex(event.data[event_idx]))
                AP_addr = AP_addr[1:]
                wirless_scan_event["AP MAC ADDRESS"] = AP_addr

            elif event.cmd == SIOCGIWESSID:
                ESSID = ""
                # LEN => 2 bytes, flags => 2 bytes, pedding => 4 bytes , Other: ESSID data
                for ssid_chr in range(8,event.data[0]+8):
                    ESSID = ESSID + chr(event.data[ssid_chr])
                wirless_scan_event["ESSID"] = ESSID

            elif event.cmd == SIOCGIWFREQ:
                freq_channel = event.data[0] | (event.data[1] << 8)
                if freq_channel >= 2400:
                    wirless_scan_event["FREQ"] = freq_channel
                else:
                    wirless_scan_event["CHANNEL"] = freq_channel


            elif event.cmd == SIOCGIWRATE:
                # Pedding 4 bytes between 2 data. An bitrate size is 4 bytes.
                for bitrate_idx in range(0, event.data_len-8, 8):
                    bitrate = event.data[bitrate_idx] | (event.data[bitrate_idx+1] << 8) | (event.data[bitrate_idx+2] << 16) | (event.data[bitrate_idx+3] << 24)
                    wirless_scan_event["BITRATE"].append(bitrate)

        return wireless_scan_result[1:]

if __name__ == '__main__':
    wire_scan = wireless_scan()
    print(wire_scan.extract(wire_scan.scan('wlan2')))

    