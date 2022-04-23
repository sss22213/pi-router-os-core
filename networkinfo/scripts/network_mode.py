from uci import Uci
import os

class network_mode:
    def __init__(self):
        self.u = Uci()
        self.wire_info = self.u.get("wireless")
        self.network_info = self.u.get("network")
    
    ''' 
        interface: ethernet, wireless0, wireless1
        proto: pppoe(1), static(2), dhcp(3)
    '''
    def get_network_mode(self):
        network_mode = []
        if self.network_info["lan"]["proto"]=="pppoe":
            network_mode.append(1)
        elif self.network_info["lan"]["proto"]=="static":
            network_mode.append(2)
        elif self.network_info["lan"]["proto"]=="dhcp":
            network_mode.append(3)

        if self.network_info["wan"]["proto"]=="pppoe":
            network_mode.append(1)
        elif self.network_info["wan"]["proto"]=="static":
            network_mode.append(2)
        elif self.network_info["wan"]["proto"]=="dhcp":
            network_mode.append(3)
        
        return network_mode

if __name__ == '__main__':
    net_mode = network_mode()
    print(net_mode.get_network_mode())
    pass