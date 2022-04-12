'''
/* Routing table calls. */
#define SIOCADDRT	0x890B		/* add routing table entry	*/
#define SIOCDELRT	0x890C		/* delete routing table entry	*/
#define SIOCRTMSG	0x890D		/* unused			*/

/* Socket configuration controls. */
#define SIOCGIFNAME	0x8910		/* get iface name		*/
#define SIOCSIFLINK	0x8911		/* set iface channel		*/
#define SIOCGIFCONF	0x8912		/* get iface list		*/
#define SIOCGIFFLAGS	0x8913		/* get flags			*/
#define SIOCSIFFLAGS	0x8914		/* set flags			*/
#define SIOCGIFADDR	0x8915		/* get PA address		*/
#define SIOCSIFADDR	0x8916		/* set PA address		*/
#define SIOCGIFDSTADDR	0x8917		/* get remote PA address	*/
#define SIOCSIFDSTADDR	0x8918		/* set remote PA address	*/
#define SIOCGIFBRDADDR	0x8919		/* get broadcast PA address	*/
#define SIOCSIFBRDADDR	0x891a		/* set broadcast PA address	*/
#define SIOCGIFNETMASK	0x891b		/* get network PA mask		*/
#define SIOCSIFNETMASK	0x891c		/* set network PA mask		*/
#define SIOCGIFMETRIC	0x891d		/* get metric			*/
#define SIOCSIFMETRIC	0x891e		/* set metric			*/
#define SIOCGIFMEM	0x891f		/* get memory address (BSD)	*/
#define SIOCSIFMEM	0x8920		/* set memory address (BSD)	*/
#define SIOCGIFMTU	0x8921		/* get MTU size			*/
#define SIOCSIFMTU	0x8922		/* set MTU size			*/
#define SIOCSIFNAME	0x8923		/* set interface name */
#define	SIOCSIFHWADDR	0x8924		/* set hardware address 	*/
#define SIOCGIFENCAP	0x8925		/* get/set encapsulations       */
#define SIOCSIFENCAP	0x8926		
#define SIOCGIFHWADDR	0x8927		/* Get hardware address		*/
#define SIOCGIFSLAVE	0x8929		/* Driver slaving support	*/
#define SIOCSIFSLAVE	0x8930
#define SIOCADDMULTI	0x8931		/* Multicast address lists	*/
#define SIOCDELMULTI	0x8932
#define SIOCGIFINDEX	0x8933		/* name -> if_index mapping	*/
#define SIOGIFINDEX	SIOCGIFINDEX	/* misprint compatibility :-)	*/
#define SIOCSIFPFLAGS	0x8934		/* set/get extended flags set	*/
#define SIOCGIFPFLAGS	0x8935
#define SIOCDIFADDR	0x8936		/* delete PA address		*/
#define	SIOCSIFHWBROADCAST	0x8937	/* set hardware broadcast addr	*/
#define SIOCGIFCOUNT	0x8938		/* get number of devices */

#define SIOCGIFBR	0x8940		/* Bridging support		*/
#define SIOCSIFBR	0x8941		/* Set bridging options 	*/

#define SIOCGIFTXQLEN	0x8942		/* Get the tx queue length	*/
#define SIOCSIFTXQLEN	0x8943		/* Set the tx queue length 	*/

/* SIOCGIFDIVERT was:	0x8944		Frame diversion support */
/* SIOCSIFDIVERT was:	0x8945		Set frame diversion options */

#define SIOCETHTOOL	0x8946		/* Ethtool interface		*/

#define SIOCGMIIPHY	0x8947		/* Get address of MII PHY in use. */
#define SIOCGMIIREG	0x8948		/* Read MII PHY register.	*/
#define SIOCSMIIREG	0x8949		/* Write MII PHY register.	*/

#define SIOCWANDEV	0x894A		/* get/set netdev parameters	*/

#define SIOCOUTQNSD	0x894B		/* output queue size (not sent only) */
#define SIOCGSKNS	0x894C		/* get socket network namespace */

/* ARP cache control calls. */
		    /*  0x8950 - 0x8952  * obsolete calls, don't re-use */
#define SIOCDARP	0x8953		/* delete ARP table entry	*/
#define SIOCGARP	0x8954		/* get ARP table entry		*/
#define SIOCSARP	0x8955		/* set ARP table entry		*/

/* RARP cache control calls. */
#define SIOCDRARP	0x8960		/* delete RARP table entry	*/
#define SIOCGRARP	0x8961		/* get RARP table entry		*/
#define SIOCSRARP	0x8962		/* set RARP table entry		*/

/* Driver configuration calls */

#define SIOCGIFMAP	0x8970		/* Get device parameters	*/
#define SIOCSIFMAP	0x8971		/* Set device parameters	*/

/* DLCI configuration calls */

#define SIOCADDDLCI	0x8980		/* Create new DLCI device	*/
#define SIOCDELDLCI	0x8981		/* Delete DLCI device		*/

#define SIOCGIFVLAN	0x8982		/* 802.1Q VLAN support		*/
#define SIOCSIFVLAN	0x8983		/* Set 802.1Q VLAN options 	*/

/* bonding calls */

#define SIOCBONDENSLAVE	0x8990		/* enslave a device to the bond */
#define SIOCBONDRELEASE 0x8991		/* release a slave from the bond*/
#define SIOCBONDSETHWADDR      0x8992	/* set the hw addr of the bond  */
#define SIOCBONDSLAVEINFOQUERY 0x8993   /* rtn info about slave state   */
#define SIOCBONDINFOQUERY      0x8994	/* rtn info about bond state    */
#define SIOCBONDCHANGEACTIVE   0x8995   /* update to a new active slave */
			
/* bridge calls */
#define SIOCBRADDBR     0x89a0		/* create new bridge device     */
#define SIOCBRDELBR     0x89a1		/* remove bridge device         */
#define SIOCBRADDIF	0x89a2		/* add interface to bridge      */
#define SIOCBRDELIF	0x89a3		/* remove interface from bridge */

/* hardware time stamping: parameters in linux/net_tstamp.h */
#define SIOCSHWTSTAMP	0x89b0		/* set and get config		*/
#define SIOCGHWTSTAMP	0x89b1		/* get config			*/
'''
import socket
import struct
import array
import fcntl
import sys
import os

SIOCGIFNAME = 0x8910		# get iface name
SIOCSIFLINK = 0x8911		# get iface channel
SIOCGIFCONF = 0x8912		# get iface list
SIOCGIFFLAGS = 0x8913		# get flags
SIOCGIFADDR = 0x8915        # get PA address
SIOCGIFDSTADDR = 0x8917		# get remote PA address
SIOCGIFBRDADDR = 0x8919		# get broadcast PA address
SIOCGIFNETMASK = 0x891b		# get network PA mask
SIOCGIFMETRIC = 0x891d		# get metric
SIOCGIFMTU = 0x8921	        # get MTU size
SIOCGIFTXQLEN = 0x8942		# Get the tx queue length
SIOCGMIIREG = 0x8948		# Read MII PHY register
SIOCGIFHWADDR = 0x8927      # get MAC address
IFNAMSIZ = 16
IFHWADDRLEN = 6

class netdevice:
    def __init__(self):
        self.fd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    '''
    /*
    * Interface request structure used for socket
    * ioctl's.  All interface ioctl's must have parameter
    * definitions which begin with ifr_name.  The
    * remainder may be interface specific.
    */
    struct ifreq {
    #define	IFNAMSIZ	16
    #define IFHWADDRLEN	6
        union
        {
            char	ifrn_name[IFNAMSIZ];		/* Interface name */
        } ifr_ifrn;
        
        union {
            struct	sockaddr ifru_addr;
            struct	sockaddr ifru_dstaddr;
            struct	sockaddr ifru_broadaddr;
            struct	sockaddr ifru_netmask;
            struct  sockaddr ifru_hwaddr;
            short	ifru_flags;
            int	ifru_ivalue;
            int	ifru_mtu;
            struct  ifmap ifru_map;
            char	ifru_slave[IFNAMSIZ];	/* Just fits the size */
            char	ifru_newname[IFNAMSIZ];
            void __user *	ifru_data;
            struct	if_settings ifru_settings;
        } ifr_ifru;
    };
    '''

    def hex_to_string(self, val):
        res = ""
        big=val//16
        little=val%16

        if big < 10:
                res += (chr(big+0x30))
        else:
                res += (chr(big-10+97))

        if little < 10:
                res += (chr(little+0x30))
        else:
                res += (chr(little-10+97))
        return res

    def is_interface(self, iface):
        interface_list = self.get_interface_name()
        if iface in interface_list:
            return True
        else:
            return False

    def get_interface_name(self, max_queue=20):
        iface_name = []

        suruct_size = 40

        struct_byte = suruct_size * max_queue

        names = array.array('B')
        for i in range(0, struct_byte):
            names.append(0)

        # array.buffer_info = [address, length]
        input_buffer = struct.pack('iL', struct_byte, names.buffer_info()[0])

        output_buffer = fcntl.ioctl(self.fd.fileno(), SIOCGIFCONF, input_buffer)

        output_size = struct.unpack('iL', output_buffer)[0]

        namestr = names.tobytes()
        for i in range(0, output_size, suruct_size):
            iface_name.append(namestr[i:i+IFNAMSIZ].decode().split('\0',1)[0])

        return iface_name

    def get_ip_addr(self, iface):
        # A interface require a struct ifreq. A struct ifreq size is 40 bytes(char).
        input_buffer = struct.pack('40s', bytes(iface, 'utf8'))

        output_buffer = fcntl.ioctl(self.fd.fileno(), SIOCGIFADDR, input_buffer)

        return socket.inet_ntoa(output_buffer[20:24])

    def get_netmask(self, iface):
        input_buffer = struct.pack('40s', bytes(iface, 'utf8'))

        output_buffer = fcntl.ioctl(self.fd.fileno(), SIOCGIFNETMASK, input_buffer)

        return socket.inet_ntoa(output_buffer[20:24])

    def get_mac_address(self, iface):
        input_buffer = struct.pack('40s', bytes(iface, 'utf8'))

        output_buffer = fcntl.ioctl(self.fd.fileno(), SIOCGIFHWADDR, input_buffer)

        mac_addr = ""

        for mac_addr_idx in range(0,6):
            mac_addr += self.hex_to_string(output_buffer[mac_addr_idx+18])
            mac_addr += ":"

        return mac_addr[:-1]
        
    def close(self):
        self.fd.close()

if __name__ == '__main__':
    net = netdevice()
    print(net.get_interface_name())
    print(net.get_ip_addr('wlan2'))
    print(net.get_netmask('wlan2'))
    print(net.get_mac_address('wlan2'))
    net.close()