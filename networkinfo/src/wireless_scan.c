#include "wireless_scan.h"

/**
 * @brief Extract data of stream
 * 
 * @param wirelss_scan_event 
 * @param stream 
 * @param stream_len 
 * @return true Extract success.
 * @return false Extract failed.
 */
static inline bool wireless_scan_extract_stream(struct _wirelss_scan_event *wirelss_scan_event, uint8_t *stream, int stream_len)
{   
    struct iw_event i_event;

    uint8_t *ptr_end = stream + stream_len;
    uint8_t *ptr_start = stream;
    uint8_t *ptr_cur = ptr_start;
    
    struct _wirelss_scan_event *ptr_wirelss_scan_event = wirelss_scan_event;

    while (ptr_cur < ptr_end) {
        memset(&i_event, 0, sizeof(struct iw_event));

        memcpy(&(i_event.len), ptr_cur, sizeof(i_event.len));
        if (i_event.len < IW_EV_LCP_PK_LEN) {
            ptr_cur++;
            continue;
        }

        memcpy(&(i_event.cmd), ptr_cur+sizeof(i_event.cmd), sizeof(i_event.cmd));
        
        if (i_event.cmd >= SIOCIWLASTPRIV) {
            // Offset to next event
            ptr_cur+= i_event.len;
            continue;
        } else {
			memcpy(&(ptr_wirelss_scan_event->i_event.cmd), &(i_event.cmd), sizeof(i_event.cmd));
			memcpy(&(ptr_wirelss_scan_event->i_event.len), &(i_event.len), sizeof(i_event.len));
			memcpy(&(ptr_wirelss_scan_event->buf), ptr_cur+8, sizeof(ptr_cur[0]) * i_event.len-8);

			ptr_wirelss_scan_event++;
        }
        
        // Offset to next event
        ptr_cur += i_event.len;
    }

    return true;
}

/**
 * @brief Analysis data of stream
 * 
 * @param _wireless_scan_netinfo 
 * @param wirelss_scan_event 
 * @return true 
 * @return false 
 */
static inline bool wireless_scan_print_stream(struct _wireless_scan_netinfo *_wireless_scan_netinfo, struct _wirelss_scan_event *wirelss_scan_event)
{
	struct _wirelss_scan_event *ptr_wirelss_scan_event = wirelss_scan_event;

	struct _netinfo_wireless *ptr_netinfo_wireless = _wireless_scan_netinfo->netinfo_interface;
    _wireless_scan_netinfo->ptr_netinfo_interface_start = _wireless_scan_netinfo->netinfo_interface+1;

	int idx = 0;

	union iwreq_data u;

    uint16_t freq_channel = 0;

    uint32_t bitrate = 0;

    int *ptr_bitrate = NULL;

	while (ptr_wirelss_scan_event->i_event.cmd) {
		switch (ptr_wirelss_scan_event->i_event.cmd) {
        // AP MAC address event
		case SIOCGIWAP:
			ptr_netinfo_wireless++;

            ptr_bitrate = ptr_netinfo_wireless->bit_rate_scanning;

			memcpy(&u, ptr_wirelss_scan_event->buf, sizeof(struct sockaddr));
			for (idx = 0; idx < 6; idx++) {
				sprintf(ptr_netinfo_wireless->ap_mac_address+3*idx, "%02X:", u.addr.sa_data[idx]);
			}
			ptr_netinfo_wireless->ap_mac_address[17] = '\0';
		break;

        // ESSID event
		case SIOCGIWESSID:
			// LEN => 2 bytes, flags => 2 bytes, pedding => 4 bytes , Other: ESSID data
			memcpy(ptr_netinfo_wireless->essid, ptr_wirelss_scan_event->buf+2+2+4, sizeof(uint8_t)*ptr_wirelss_scan_event->buf[0]);
			ptr_netinfo_wireless->essid[2+2+4+sizeof(uint8_t)*ptr_wirelss_scan_event->buf[0]] = '\0';
		break;

        // Frequency and channel event
		case SIOCGIWFREQ:
            // Frequency or channel size is 2 bytes.
            memcpy(&freq_channel, ptr_wirelss_scan_event->buf, sizeof(uint8_t)*2);

            // When the value more than 2400, we can get frequency. Otherwise, get channel.
            if (freq_channel >= 2400) {
                ptr_netinfo_wireless->frequency = (float)freq_channel / 1000.0;
            } else {
                ptr_netinfo_wireless->channel = (int)freq_channel;
            }
        break;

        // Bitrate event
        case SIOCGIWRATE:
            // Pedding 4 bytes between 2 data. An bitrate size is 4 bytes.
            for (idx = 0; idx < ptr_wirelss_scan_event->i_event.len-8; idx+=4+4) {
                memcpy(&bitrate, ptr_wirelss_scan_event->buf+idx, sizeof(uint8_t)*4);
                *ptr_bitrate = bitrate;
                ptr_netinfo_wireless->bit_rate_len++;
                ptr_bitrate++;
            }
        break;
 		
		default:
		break;
		}
		ptr_wirelss_scan_event++;
	}

    _wireless_scan_netinfo->ptr_netinfo_interface_end = ptr_netinfo_wireless;

    return true;
}

bool wireless_get_scan_result(struct _wireless_scan_netinfo *wireless_scan_netinfo, const char *iface)
{
    int fd = 0;

    int ret = 0;
    (void)ret;

    int buflen = sizeof(uint8_t) * IW_SCAN_MAX_DATA;
    int scan_buflen = sizeof(uint8_t) * IW_SCAN_MAX_DATA * 2;

	struct _wirelss_scan_event wirelss_scan_event[MAXIMUM_WIRELSS_SCAN_EVENT_LENGTH] = {0};

    uint8_t raw_buf[IW_SCAN_MAX_DATA*10] = {0};

    struct iwreq iw_req;

    struct iw_scan_req iwscan_req;
    memset(&iwscan_req, 0, sizeof(struct iw_scan_req));
    iw_req.u.data.pointer = (caddr_t)&iwscan_req;
    iw_req.u.data.length = sizeof(iwscan_req);

    // Open an socket.
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    strncpy(iw_req.ifr_name, iface, IFNAMSIZ);

    // Send wireless scanning system call.
    ret = ioctl(fd, SIOCSIWSCAN, &iw_req);
    if (ret == -1) {
        return false;
    }

    // Waiting for scanning complete.
    sleep(5);
    
    // Get scanning result.
    do {
        buflen *= 2;
        iw_req.u.data.pointer = raw_buf;
        iw_req.u.data.flags = 0;
        iw_req.u.data.length = buflen;
        ret = ioctl(fd, SIOCGIWSCAN, &iw_req);
        scan_buflen = iw_req.u.data.length;
    } while (buflen < scan_buflen);

    wireless_scan_extract_stream(wirelss_scan_event, raw_buf, buflen);

	wireless_scan_print_stream(wireless_scan_netinfo, wirelss_scan_event);

    close(fd);

    return true;
}